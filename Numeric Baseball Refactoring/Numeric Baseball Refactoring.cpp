//=================================================================
// 제목 : 객체지향 기반 숫자야구 게임 (Refactored for Portfolio)
//=================================================================

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <conio.h>
#include <Windows.h>

using namespace std;

// [Utility] 콘솔 제어용 클래스
class ConsoleHelper {
public:
    static void gotoXY(int x, int y) {
        COORD Pos = { (SHORT)(x - 1), (SHORT)(y - 1) };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
    }

    static void clearScreen() {
        system("cls");
    }
};

// [Logic] 게임 로직 클래스
class BaseballGameEngine {
private:
    int digitCount;
    vector<int> targetNumbers;
    int tryCount;

public:
    BaseballGameEngine(int level) : digitCount(level), tryCount(0) {}

    void generateNumbers() {
        targetNumbers.clear();
        vector<int> candidates = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        random_device rd;
        mt19937 g(rd());
        shuffle(candidates.begin(), candidates.end(), g);

        for (int i = 0; i < digitCount; ++i) {
            targetNumbers.push_back(candidates[i]);
        }
    }

    struct Result {
        int strike = 0;
        int ball = 0;
        int out = 0;
        bool isWin = false;
    };

    Result calculateResult(const vector<int>& userNumbers) {
        Result res;
        tryCount++;

        for (int i = 0; i < digitCount; ++i) {
            bool found = false;
            for (int j = 0; j < digitCount; ++j) {
                if (userNumbers[i] == targetNumbers[j]) {
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

    int getDigitCount() const { return digitCount; }
    int getTryCount() const { return tryCount; }
};

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
            if (check[num]) return false;
            check[num] = true;
            outNumbers.push_back(num);
        }
        return true;
    }
};

// [Manager] 게임 흐름 관리 클래스
class GameManager {
private:
    // 메인 메뉴: 종료를 3번으로 변경
    int showMainMenu() {
        ConsoleHelper::clearScreen();
        ConsoleHelper::gotoXY(30, 5); cout << "== 객체지향 숫자야구 ==";
        ConsoleHelper::gotoXY(30, 7); cout << "1. 게임 시작";
        ConsoleHelper::gotoXY(30, 8); cout << "2. 게임 설명";
        ConsoleHelper::gotoXY(30, 9); cout << "3. 종료"; // 4 -> 3번으로 변경
        ConsoleHelper::gotoXY(30, 11); cout << "선택: ";

        string input;
        getline(cin, input);
        if (input == "1") return 1;
        if (input == "2") return 2;
        if (input == "3") return 3;
        return 0;
    }

    int selectLevel() {
        string errorMessage = "";
        while (true) {
            ConsoleHelper::clearScreen();
            ConsoleHelper::gotoXY(30, 5); cout << "== 난이도 선택 ==";
            ConsoleHelper::gotoXY(30, 7); cout << "1. 초급 (3자리)";
            ConsoleHelper::gotoXY(30, 8); cout << "2. 중급 (4자리)";
            ConsoleHelper::gotoXY(30, 9); cout << "3. 고급 (5자리)";

            if (!errorMessage.empty()) {
                ConsoleHelper::gotoXY(30, 13); cout << errorMessage;
            }

            ConsoleHelper::gotoXY(30, 11); cout << "선택:       ";
            ConsoleHelper::gotoXY(36, 11);

            string input;
            getline(cin, input);

            if (input == "1") return 3;
            else if (input == "2") return 4;
            else if (input == "3") return 5;
            else errorMessage = "<!> 1, 2, 3 중에서만 선택해주세요!";
        }
    }

    // [수정됨] 페이지 기능이 적용된 게임 설명
    void showTutorial() {
        vector<string> pages;
        pages.push_back(
            "== [1페이지] 게임 개요 ==\n\n"
            " 숫자 야구 게임에 오신 것을 환영합니다!\n"
            " 컴퓨터가 생각한 '비밀 숫자'를 맞추는 게임입니다.\n\n"
            " - 각 숫자는 0~9 사이의 정수입니다.\n"
            " - 중복된 숫자는 없습니다.\n"
            " - 난이도에 따라 자릿수가 달라집니다.\n"
        );
        pages.push_back(
            "== [2페이지] 용어 설명 ==\n\n"
            " 판정 기준은 다음과 같습니다.\n\n"
            " [S] Strike (스트라이크)\n"
            "  -> 숫자와 위치가 모두 맞았을 때\n\n"
            " [B] Ball (볼)\n"
            "  -> 숫자는 맞지만, 위치가 틀렸을 때\n\n"
            " [O] Out (아웃)\n"
            "  -> 일치하는 숫자가 하나도 없을 때\n"
        );
        pages.push_back(
            "== [3페이지] 예시 ==\n\n"
            " 정답이 [1 2 3] 일 때,\n\n"
            " - 유저 입력: [1 2 3] -> 3 Strike (정답!)\n"
            " - 유저 입력: [1 3 2] -> 1 Strike 2 Ball\n"
            " - 유저 입력: [4 5 6] -> 3 Out\n\n"
            " 자, 이제 게임을 시작해 볼까요?\n"
        );

        int currentPage = 0;
        while (true) {
            ConsoleHelper::clearScreen();
            ConsoleHelper::gotoXY(10, 5);
            cout << pages[currentPage];

            ConsoleHelper::gotoXY(10, 18);
            cout << "------------------------------------------------";
            ConsoleHelper::gotoXY(10, 19);
            cout << "PAGE [ " << (currentPage + 1) << " / " << pages.size() << " ]";
            ConsoleHelper::gotoXY(30, 19);
            cout << "이동: <- / ->  |  종료: ESC";

            int key = _getch();
            if (key == 224) {
                key = _getch();
                if (key == 75 && currentPage > 0) currentPage--;
                if (key == 77 && currentPage < pages.size() - 1) currentPage++;
            }
            else if (key == 27) break;
        }
    }

    // 최대 자릿수 제한 (limit 매개변수 활용)
    bool inputNumberWithEsc(int x, int y, int limitLength, string& outInput) {
        outInput = "";
        ConsoleHelper::gotoXY(x, y);

        while (true) {
            int ch = _getch();

            if (ch == 27) { // ESC
                ConsoleHelper::gotoXY(x + 15, y);
                cout << " <SYSTEM> 정말 종료하시겠습니까? (Y/N) ";
                int confirm = _getch();
                if (confirm == 'y' || confirm == 'Y') return false;
                else {
                    ConsoleHelper::gotoXY(x + 15, y);
                    cout << "                                        ";
                    ConsoleHelper::gotoXY(x + outInput.length(), y);
                    continue;
                }
            }
            else if (ch == 13) { // Enter
                // 자릿수가 부족하면 입력을 안 받아줌 (선택사항: 필요 없다면 이 if문 삭제)
                if (outInput.length() < limitLength) continue;
                return true;
            }
            else if (ch == 8) { // Backspace
                if (!outInput.empty()) {
                    outInput.pop_back();
                    cout << "\b \b";
                }
            }
            else if (isdigit(ch)) {
                // 현재 입력된 길이가 제한(limitLength)보다 작을 때만 입력 허용
                if (outInput.length() < limitLength) {
                    outInput += (char)ch;
                    cout << (char)ch;
                }
                // 제한을 넘으면 아무 일도 안 함 (화면에 안 찍힘)
            }
        }
    }

    void playGame(int level) {
        BaseballGameEngine engine(level);
        engine.generateNumbers();

        ConsoleHelper::clearScreen();
        ConsoleHelper::gotoXY(10, 2);
        cout << "게임 시작! (" << level << "자리 숫자) [종료하려면 ESC]";

        int line = 4;
        while (true) {
            ConsoleHelper::gotoXY(10, line);
            cout << "[" << engine.getTryCount() + 1 << "회차] 입력: ";

            string input;

            // level(난이도) 만큼만 입력을 받도록 제한
            if (!inputNumberWithEsc(26, line, level, input)) {
                ConsoleHelper::gotoXY(10, line + 2);
                cout << "게임을 종료하고 메인으로 돌아갑니다...";
                Sleep(1000);
                break;
            }
            cout << endl;

            vector<int> userNumbers;
            if (!InputValidator::parseInput(input, level, userNumbers)) {
                ConsoleHelper::gotoXY(40, line);
                cout << "<!> 잘못된 입력입니다";
                continue;
            }

            auto res = engine.calculateResult(userNumbers);
            ConsoleHelper::gotoXY(40, line);
            cout << "Result: " << res.strike << "S " << res.ball << "B " << res.out << "O";

            if (res.isWin) {
                ConsoleHelper::gotoXY(10, line + 2);
                cout << "축하합니다! " << engine.getTryCount() << "회 만에 정답을 맞췄습니다!";
                _getch();
                break;
            }

            line++;
            if (line > 20) {
                ConsoleHelper::clearScreen();
                line = 4;
            }
        }
    }

public:
    void run() {
        while (true) {
            int menu = showMainMenu();
            if (menu == 3) break;
            if (menu == 1) {
                int level = selectLevel();
                playGame(level);
            }
            else if (menu == 2) {
                showTutorial();
            }
        }
    }
};

int main() {
    GameManager game;
    game.run();
    return 0;
}