#pragma once

// 1. 게임 설정 상수 (여기만 고치면 4구, 5구 야구로 확장 가능!)
const int MAX_GAME_DIGITS = 9;      // 최대 9자리까지 지원 (넉넉하게)
const int DEFAULT_GAME_LEVEL = 4;   // 기본 게임 난이도
const int NAME_LEN = 20;            // 닉네임 최대 길이

enum PacketID {
    PT_LOGIN = 1,
    PT_GUESS = 2,
    PT_RESULT = 3
};

#pragma pack(push, 1)

struct PacketHeader {
    short size;
    short id;
};

// [로그인]
struct LoginPacket {
    PacketHeader header;
    char name[NAME_LEN]; // 상수 활용
};

// [숫자 던지기] - 여기가 핵심!
struct GuessPacket {
    PacketHeader header;

    int count;                  // "나 이번에 3개 보낸다" or "4개 보낸다" 알려줌
    int numbers[MAX_GAME_DIGITS]; // 배열은 최대로 잡아둠 (낭비 좀 있어도 안전이 최고)
};

// [결과 통보]
struct ResultPacket {
    PacketHeader header;
    int strikes;
    int balls;
    int out;
    bool isWin;
};

#pragma pack(pop)