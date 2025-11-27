#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

#include "../GamePacket.h"

using namespace std;

struct Result {
    int strike = 0;
    int ball = 0;
    int out = 0;
    bool isWin = false;
};

// [Logic] 게임 로직 클래스
class GameEngine {
public:
    GameEngine(int level = DEFAULT_GAME_LEVEL);
    ~GameEngine();

    // 정답 생성
    void GenerateAnswer();
    // 정답 계산
    Result CalculateResult(const vector<int>& userNum);
    // 게임 초기화 (정답 재생성)
    void Reset();

    int getDigitCount() const { return digitCount; }
    int getTryCount() const { return tryCount; }

private:
    int digitCount;
    vector<int> ansNum;
    int tryCount;
};