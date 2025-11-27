#pragma once
#include <WinSock2.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

#include "GameEngine.h"

#pragma comment(lib, "ws2_32.lib")

class NetworkManager
{
// 클래스의 외부에서 접근 가능 ()
public:
	NetworkManager();		// 생성자 : 초기화 역할
	~NetworkManager();		// 소멸자 : 사용하지 않는 자원의 해제
	
	bool InitServer();		// 서버용 소켓을 만드는 함수 (초기화 수행)	
	bool Bind(int port);	// 전화번호(Port)를 붙이는 함수
	bool StartListen(int backlog = 5);	// 연결 대기시작 (backlog = 대기열 개념)
	bool AcceptClient();	// 클라이언트의 접속을 수락하는 함수

// 클래스 내부 자료 (*업무툴)
private:
	WSADATA m_wsaData;
	SOCKET m_serverSocket;
	SOCKADDR_IN m_serverAddr; // 주소 정보(IP, Port)를 담을 구조체 (*가방)

	// 접속된 클라이언트의 정보를 담을 변수들
	SOCKET m_clientSocket;      // 손님과 대화할 전용 소켓
	SOCKADDR_IN m_clientAddr;   // 손님의 주소 정보 (IP 등)

	static void ProcessClient(SOCKET clientSock, SOCKADDR_IN clientAddr);

	// 모든 클라이언트가 공유할 게임 엔진
	static GameEngine m_gameEngine;
	
	// 서버에 접속한 모든 클라이언트 소켓 보관함 (접속자 명단)
	static std::vector<SOCKET> m_clients;

	// 명단 수정할 때 충돌 방지용 자물쇠
	static std::mutex m_lock;
};
