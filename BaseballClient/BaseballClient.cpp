// BaseballClient.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h> // inet_pton 사용을 위해 필요
#include "../GamePacket.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

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
    SOCKET hSocket = socket(AF_INET, SOCK_STREAM, 0);
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

    if (sendResult == SOCKET_ERROR) {
        cout << "로그인 패킷 전송 실패!" << endl;
    }
    else {
        cout << "로그인 요청 전송 완료! (" << sendResult << " bytes)" << endl;
    }

    // [시나리오] 유저가 숫자 3개(1, 2, 3)를 입력했다고 가정
    cout << "\n>>> 숫자 3개를 던집니다! (1, 2, 3) <<<" << endl;

    GuessPacket guessPkt;
    guessPkt.header.id = PT_GUESS;
    guessPkt.header.size = sizeof(GuessPacket);

    // ★ 유동적인 부분 설정
    guessPkt.count = 3; // "나 숫자 3개 채웠어!"
    guessPkt.numbers[0] = 1;
    guessPkt.numbers[1] = 2;
    guessPkt.numbers[2] = 3;

    // (주의: 나머지 numbers[3]~[8]은 쓰레기 값이 들어있어도 상관없음. 서버가 count만큼만 읽을 거니까!)

    sendResult = send(hSocket, (char*)&guessPkt, sizeof(GuessPacket), 0);

    if (sendResult == SOCKET_ERROR) {
        cout << "숫자 전송 실패!" << endl;
    }
    else {
        cout << "숫자 데이터 전송 완료! (" << sendResult << " bytes)" << endl;
    }

    // 연결 끊지 않고 대기
    while (true) {}

    closesocket(hSocket);
    WSACleanup();

    return 0;
}
