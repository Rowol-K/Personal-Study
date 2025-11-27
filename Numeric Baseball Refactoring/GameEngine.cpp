#include "GameEngine.h"

// 생성자 구현
GameEngine::GameEngine(int level) : digitCount(level), tryCount(0) { GenerateAnswer(); } // 생성되자마자 정답 만들기
GameEngine::~GameEngine() {}

// 정답 생성
void GameEngine::GenerateAnswer() {
    ansNum.clear(); // 초기화
    tryCount = 0; // 시도 횟수 초기화

    vector<int> num = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    random_device rd;
    mt19937 g(rd());
    shuffle(num.begin(), num.end(), g);

    for (int i = 0; i < digitCount; ++i) ansNum.push_back(num[i]);

    // 정답 확인 (※디버그 용도. 본 게임으로 넘어가면 해당 줄은 주석처리 할 것)
    cout << "Debug: 정답 생성 완료 ( ";
    for (int n : ansNum) { cout << n << " "; }
    cout << " ) - 자릿수: " << digitCount << endl;
}

// 정답 계산
Result GameEngine::CalculateResult(const vector<int>& userNum) {
    Result res;
    tryCount++;

    for (int i = 0; i < digitCount; ++i) {
        bool found = false;
        for (int j = 0; j < digitCount; ++j) {
            if (userNum[i] == ansNum[j]) {
                if (i == j) res.strike++;
                else res.ball++;
                found = true;
                break;
            }
        }
        if (!found) res.out++;
    }

    if (res.strike == digitCount) res.isWin = true;

    return res;
}

// 게임 초기화 (정답 재생성)
void GameEngine::Reset() {
    GenerateAnswer();
}