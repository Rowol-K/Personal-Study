#include "Network Manager.h"
#include <ws2tcpip.h>
#include "../GamePacket.h"

using namespace std;

// 생성자 : 프로그램 시작 시, 소켓 초기화 및 자원 할당 (소켓 활성화)
NetworkManager::NetworkManager() {
	// 버전 초기화 (2.2)
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
		cerr << "Error: WinSock 초기화 실패 (WSAStartup)" << endl;
	}
	else {
		cout << "System: WinSock 초기화 완료." << endl;
	}

	// 소켓 변수 초기화
	m_serverSocket = INVALID_SOCKET;
}

// 소멸자 : 프로그램 종료 시 반환
NetworkManager::~NetworkManager() {
	if (m_serverSocket != INVALID_SOCKET) {
		closesocket(m_serverSocket);
	}
	WSACleanup(); // WinSock 해제
	cout << "System: 네트워크 리소스 해제 완료." << endl;
}

// 소켓 생성 함수
bool NetworkManager::InitServer() {
	// socket(주소체계, 소켓타입, 프로토콜)
	// AF_INET: IPv4 (인터넷)
	// SOCK_STREAM: TCP (신뢰성)
	m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_serverSocket == INVALID_SOCKET) {
		cerr << "Error: 소켓 생성 실패! 에러코드: " << WSAGetLastError() << endl;
		return false;
	}

	cout << "System: 서버 소켓 생성 성공! (Socket ID: " << m_serverSocket << ")" << endl;
	return true;
}

// 전화번호(IP/Port) 붙이기
bool NetworkManager::Bind(int port) {
	// 1. 주소 정보 구조체(가방)를 0으로 깨끗하게 지운다. (혹시 모를 쓰레기 값이 들어가지 않게 청소)
	memset(&m_serverAddr, 0, sizeof(m_serverAddr));

	// 2. 주소 정보 채우기
	m_serverAddr.sin_family = AF_INET; // IPv4 주소 체계 쓸거다

	// IP 설정: INADDR_ANY (내 컴퓨터의 어떤 IP로 들어오든 다 받겠다)
	// htonl: Host to Network Long (네트워크용 숫자로 변환)
	m_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Port 설정: 우리가 정한 포트 번호 (예: 9000)
	// htons: Host to Network Short (네트워크용 숫자로 변환)
	m_serverAddr.sin_port = htons(port);

	// 3. 진짜로 붙이기 (bind 함수 호출)
	// (SOCKADDR*) 형변환은 bind 함수가 옛날 함수라 요구하는 형식임
	if (bind(m_serverSocket, (SOCKADDR*)&m_serverAddr, sizeof(m_serverAddr)) == SOCKET_ERROR) {
		cerr << "Error: Bind 실패! (혹시 이미 켜져 있거나 사용 중인 포트 아냐?)" << endl;
		return false;
	}

	cout << "System: Port " << port << "번 바인딩 성공!" << endl;
	return true;
}

bool NetworkManager::StartListen(int backlog) {
	// listen(소켓, 대기열 크기)
	// 이 함수가 호출되는 순간, 이 소켓은 '전화벨이 울리기를 기다리는 상태'가 됨.

	// ☆ backlog: 서버가 동시에 처리할 수 있는 최대 대기열 크기 (손님이 몰렸을 때 몇 명까지 줄 세워둘지 결정)
	if (listen(m_serverSocket, backlog) == SOCKET_ERROR) {
		cerr << "Error: listen() 실패. 에러 코드: " << WSAGetLastError() << endl;
		return false;
	}

	cout << "System: Server is Listening... (대기열 크기: " << backlog << ")" << endl;
	return true;
}

bool NetworkManager::AcceptClient() {
	int clientAddrSize = sizeof(m_clientAddr);

	// 해당 출력은 main루프에서 출력하는 것이 좋음.
	// cout << "System: 클라이언트 접속 대기 중... (Accept 호출)" << endl;

	// ★ 핵심: accept()
	// 여기서 프로그램은 누군가 올 때까지 딱 멈춰있음! (Blocking)
	m_clientSocket = accept(m_serverSocket, (SOCKADDR*)&m_clientAddr, &clientAddrSize);

	if (m_clientSocket == INVALID_SOCKET) {
		cerr << "Error: Accept 실패! 에러 코드: " << WSAGetLastError() << endl;
		return false;
	}

	// 접속한 클라이언트 IP 확인해보기 (재미 요소)
	char clientIP[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &m_clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);

	cout << "System: 새로운 클라이언트가 접속하였습니다. 새로운 스레드를 만듭니다. (IP: " << clientIP << ")" << endl;

	// ★★★★★ 핵심: 스레드 생성 (멀티스레드 프로그래밍)
	// std::thread(실행할함수, 인자1, 인자2)
	thread t(ProcessClient, m_clientSocket, m_clientAddr);

	// detach: "너 알아서 일하고 퇴근해. 난 신경 끈다." (독립시킴)
	t.detach();

	return true;
}

void NetworkManager::ProcessClient(SOCKET clientSock, SOCKADDR_IN clientAddr)
{	
	// 클라이언트 IP 주소 찍기 (디버깅용)
	char clientIP[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
	cout << "Thread Started for: " << clientIP << endl;

	// 개별 클라이언트에게 계속 패킷을 송수신함
	while (true) {
		// ★★ 버퍼를 항상 초기화 (패킷의 중복방지)
		char buffer[1024];
		memset(buffer, 0, sizeof(buffer));

		// 패킷 수신
		int recvLen = recv(clientSock, buffer, sizeof(buffer), 0);

		// 연결 종료 감지
		if (recvLen <= 0) {
			cout << "Client Disconnected: " << clientIP << endl;
			break;
		}

		// ★★★★★ 패킷 뭉침 해결 로직 ★★★★★
		int processedLen = 0; // 처리한 데이터 길이
		
		while (processedLen < recvLen) {
			// 1. 남은 데이터에서 헤더 확인
			PacketHeader* header = (PacketHeader*)&buffer[processedLen];

			// 2. 패킷 크기 검증 (버퍼 오버런 방지)
			if (processedLen + header->size > recvLen) {
				break; // 잘린 패킷은 다음 recv를 기다림
			}

			// 3. 패킷 처리
			switch (header->id)
			{
			case (PT_LOGIN):
			{
				LoginPacket* pkt = (LoginPacket*)header;
				cout << "[" << clientIP << "] 님이 로그인: " << pkt->name << endl;
				break;
			}
			case (PT_GUESS):
			{
				GuessPacket* pkt = (GuessPacket*)header;
				cout << "[" << clientIP << "] 숫자 던짐: ";
				for (int i = 0; i < pkt->count; ++i) cout << pkt->numbers[i] << " ";
				cout << endl;
				break;
			}
			default:
				break;
			}

			// 4. 다음 패킷 위치로 이동! (핵심)
			processedLen += header->size;
		}
	}
}