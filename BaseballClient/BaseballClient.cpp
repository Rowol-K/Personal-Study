// BaseballClient.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <vector>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h> // inet_pton 사용을 위해 필요
#include <thread>

#include "../GamePacket.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

SOCKET hSocket;
bool inGame = true;

// [Validator] 입력 검증 클래스
class InputValidator {
public:
    static bool parseInput(string input, int digitCount, vector<int>& outNumbers) {
        outNumbers.clear();
        if (input.length() != digitCount) return false;

        bool check[10] = { false };
        for (char c : input) {
            if (!isdigit(c)) return false;
            int num = c - '0';
            if (check[num]) return false; // 중복 체크
            check[num] = true;
            outNumbers.push_back(num);
        }
        return true;
    }
};

// 서버 패킷 수신
void RecvThread() {
    while (inGame)
    {
        char buffer[1024] = { 0 };

        int recvLen = recv(hSocket, buffer, sizeof(buffer), 0);

        if (recvLen <= 0) {
            cout << "\n[System] 서버와의 연결이 끊어졌습니다." << endl;
            inGame = false;
            break;
        }
    
        // 패킷 처리
        int processedLen = 0;
        while (processedLen < recvLen)
        {
            PacketHeader* header = (PacketHeader*)&buffer[processedLen];

            // 오버 사이즈의 패킷이 오면 탈출
            if (processedLen + header->size > recvLen) break;

            if (header->id == PT_RESULT) {
                ResultPacket* resPkt = (ResultPacket*)header;

                // 메인 함수가 입력 대기상태일 때에도 메시지가 뜰 수 있게 줄바꿔서 출력
                cout << "\n--------------------------------------------" << endl;
                cout << "[Server] " << resPkt->strikes << " Strike / " << resPkt->balls << " Ball / " << resPkt->out << " Out" << endl;
            
                // 정답 처리
                if (resPkt->isWin) {
                    cout << "\n[Server] 정답자가 발생하여 게임이 종료됩니다." << endl;
                    inGame = false;
                    Sleep(3000);
                }
                cout << "\n--------------------------------------------" << endl;
                cout << "\n[입력] : ";
            }
            processedLen += header->size;
        }
    }
}

int main()
{
    // [서버와 연결과정]
    // 
    // 1. WinSock 초기화
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WinSock 초기화 실패" << endl;
        return -1;
    }

    // 2. 소켓 생성
    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (hSocket == INVALID_SOCKET) {
        cout << "소켓 생성 실패" << endl;
        return -1;
    }

    // 3. 서버 주소 설정 (전화 걸 곳)
    SOCKADDR_IN serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000); // 서버랑 같은 포트!

    // 127.0.0.1은 '내 컴퓨터(Localhost)'를 의미함
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    // 4. 연결 요청 (Connect)
    cout << "서버에 접속을 시도합니다..." << endl;
    if (connect(hSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Connect 실패! (서버가 켜져 있나요?)" << endl;
        return -1;
    }

    cout << ">>> 서버 접속 성공! <<<" << endl;


    // ※ 데이터 전송과정
    // 0. 전송할 데이터를 담을 변수 (도시락 통 준비)
    int sendResult;

    // 1. 로그인 패킷 만들기 (도시락 싸기)
    LoginPacket loginPkt;
    loginPkt.header.id = PT_LOGIN;
    loginPkt.header.size = sizeof(LoginPacket);
    strcpy_s(loginPkt.name, "Rowol"); // 유저 닉네임

    // 2. 로그인 패킷 보내기 (도시락 던지기)
    // (char*)&loginPkt : 구조체를 바이트 덩어리로 형변환해서 보냄
    sendResult = send(hSocket, (char*)&loginPkt, sizeof(LoginPacket), 0);

    // 로그인 상태 확인
    if (sendResult == SOCKET_ERROR) { cout << "로그인 패킷 전송 실패!" << endl; }
    else { cout << "로그인 요청 전송 완료! (" << sendResult << " bytes)" << endl; }

    // 게임 시작
    cout << "\n[Client] 게임을 시작합니다! (난이도 : " << DEFAULT_GAME_LEVEL << ")" << endl;

    // 서버 패킷 수신용 스레드 생성
    thread t(RecvThread);
    t.detach(); // "너 알아서 들어라, 난 입력받으러 간다"

    // 메인 스레드
    // 1. 유저 입력 받기
    cout << "\n[입력] : ";
    while (inGame) {
        string input;
        cin >> input;

        if (!inGame) {
            cout << "이미 종료된 게임입니다." << endl;
            break;
        }

        // 입력 검증 클래스를 활용하여 검증
        vector<int> userNum;
        if (!InputValidator::parseInput(input, DEFAULT_GAME_LEVEL, userNum)) {
            cout << "[Client] 잘못된 입력입니다. (" << DEFAULT_GAME_LEVEL << "자리 중복없는 숫자를 입력하세요.)" << endl;
            continue; // 다시 입력받기
        }

        // 2. 패킷 만들기
        GuessPacket guessPkt;
        guessPkt.header.id = PT_GUESS;
        guessPkt.header.size = sizeof(GuessPacket);
        guessPkt.count = DEFAULT_GAME_LEVEL;

        for (int i = 0; i < DEFAULT_GAME_LEVEL; ++i) {
            guessPkt.numbers[i] = userNum[i]; // 벡터에서 꺼내 담기
        }

        // 3. 전송 (Send)
        send(hSocket, (char*)&guessPkt, sizeof(GuessPacket), 0);
        Sleep(100);

        // 4. 결과 대기 (Recv) - 해당 부분은 스레드에 전담시켜 동시성을 확보.
        /*
        char buffer[1024];
        int recvLen = recv(hSocket, buffer, sizeof(buffer), 0);

        if (recvLen <= 0) {
            cout << "서버와 연결이 끊어졌습니다." << endl;
            break;
        }

        // 4. 결과 확인
        PacketHeader* header = (PacketHeader*)buffer;
        if (header->id == PT_RESULT) {
            ResultPacket* resPkt = (ResultPacket*)buffer;
            cout << "[Server] " << resPkt->strikes << " Strike / " << resPkt->balls << " Ball / " << resPkt->out << " Out" << endl;

            // 정답 처리
            if (resPkt->isWin) {
                cout << "\n[Server] 정답입니다!! 축하합니다!" << endl;
                Sleep(3000);
                break; // 게임 끝! (루프 탈출)
            }
        }
        */
    }

    closesocket(hSocket);
    WSACleanup();
    system("pause");

    return 0;
}
