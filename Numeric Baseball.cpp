//=================================================================< 비고 >===============================================================================
/*
프로그램 명		: 숫자 야구
만든 사람		: Rowol-K
최종 수정일		: 2022 - 09 - 20

비고  :
	이 프로그램은 Microsoft Visual Studio 2017을 기반으로 코딩 되었습니다.
	This program is cording based on Microsoft Visual Studio 2017.

출처 :	X  (수업 시간에 배운 문법들만 사용하여 구현하였습니다. 또한 일부 내용은 인터넷의 여러 예제들의 알고리즘을 통합하여 새로 만들었습니다.)
*/
//========================================================================================================================================================



// 헤더 접속
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<string.h>
#include<Windows.h>
//---------------------------------------------전역변수---------------------------------------------------------

int Max_Console_Width = 80, Max_Console_Height = 25;  //콘솔 사이즈 조정
int i, j;	//반복문 루프 제어
int x, y;	//좌표 접근 변수
int idx = 0;	//인덱스 접근 변수 (idx)
int game_switch, reset;  //메뉴 제어 스위치


int level;	//게임 난이도 (자릿수)
char Com_number[5] = { 0 };	//정답 숫자
char no_overlap[10] = { 0 };	//중복제거
char User_number[30][5] = { 0 };	// [n 번째] [사용자 숫자]
int get_num = 0;		//입력

//입력 로그 ( [찬스], [Number, S, B, O] )
int game_log[15][4] = { 0 };
int turn_log = 0;
int Number_log = 0;
int S_log = 0;
int B_log = 0;
int O_log = 0;

int score = 0;						//점수 변수
int highest_score[3] = { 0 };		//최고 점수 (0 - 쉬움 / 1 - 보통 / 2 - 어려움)


int Strike = 0, Ball = 0, Out = 0;  //스트라이크, 볼, 아웃
int chance, turn;	// 남은 기회, 진행 턴수
int game_count[50];		//결과창 턴 표시
int result_pos = 6;		//결과창 출력 위치

//------------------------------------------------전역변수----------------------------------------------------






//커서 좌표 이동	●
void gotoxy(int x, int y)
{
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//스크린 구간 클리어
void cls_area(int x, int y, int a, int b) { //        (x,y => 시작 좌표 / a,b => 종료 좌표)
	int i, j;
	gotoxy(x, y);
	for (i = 0; i < (b - y); i++) {
		for (j = 0; j < (a - x); j++) {
			gotoxy(x + j, y + i);
			printf(" ");
		}
	}
	// a - x : 클리어 구간 x 길이
	// b - y : 클리어 구간 y 길이
}

// ( Int --> Char ) 변환 함수
char Int_to_Char(int x) {
	return x + '0';
}

//게임 메인화면    ●    메인화면, 메뉴선택
int Title() {
	//메뉴 디자인
	gotoxy(31, 5);
	printf("●○-숫자 야구-○●");
	gotoxy(33, 6);
	printf("--Select Menu--");

	//메뉴 리스트
	for (j = 0; j < 5; j++) {
		gotoxy(32, j + 7);
		switch (j) {
		case 0:
			printf("1. 게임 시작");
			break;
		case 1:
			printf("2. 게임 방법");
			break;
		case 2:
			printf("3. 최고 점수 확인");
			break;
		case 3:
			printf("4. 게임 종료");
			break;
		case 4:
			printf("================");
			gotoxy(32, 12);
			printf("메뉴 선택 : ");
			break;
		}
	}

	//메뉴 입력
	game_switch = _getch();
	game_switch = game_switch - '0';
	//scanf_s("%d", &game_switch);
	system("cls");
	return game_switch;
}

//게임 설명    ●
void Game_Explain() {
	int s = 0; //문자열 읽어오기 변수
	int e = 8, h = 40, y = 4; //출력문 위치 제어

	//개요
	char Explain_summary_0[60] = "숫자 야구는 정해진 숫자를 볼과 아웃, 스트라이크를 통하여 정";
	char Explain_summary_1[60] = "해진 기회 안에 맞추는 추리 게임입니다. 사용자는 자신이 게임";
	char Explain_summary_2[60] = "할 게임의 판 수와 자릿수를 지정할 수 있으며, 지정된 자리 수";
	char Explain_summary_3[60] = "에 따라 맞출 수 있는 기회가 다르게 주어집니다.주어진 기회";
	char Explain_summary_4[60] = "안에서 볼과 아웃, 스트라이크를 이용하여 추리하고, 지정된";
	char Explain_summary_5[60] = "숫자를 맞추면 승리하게 됩니다.";

	//게임 방법
	char Explain_way_0[60] = "메뉴를 통하여 문제의 총 자릿수를 입력 받으면, 각각의 자릿수";
	char Explain_way_1[60] = "에 대해 숫자를 입력할 수 있게 됩니다. 입력한 숫자가 모든";
	char Explain_way_2[60] = "정답 숫자와 맞지 않을 경우는 \"아웃\"으로 나타냅니다. 입력한";
	char Explain_way_3[60] = "숫자와 정답 숫자의 숫자는 같으나, 자릿수가 같지 않은 경우는";
	char Explain_way_4[60] = "\"볼\"로 나타냅니다. 입력한 숫자가 정답 숫자의 숫자와 자릿수";
	char Explain_way_5[60] = "모두 동일하면 \"스트라이크\"입니다.";

	//점수 계산
	char Explain_score_0[60] = "숫자 야구는 추리 기반의 게임으로, 빠르고 정확하게 답을 찾는";
	char Explain_score_1[60] = "것이 중요합니다. 아웃은 0점 이며, \"볼\"의 갯수마다 50점, \"스";
	char Explain_score_2[60] = "트라이크\"는 100점으로 계산됩니다. 정해진 횟수보다 이르게";
	char Explain_score_3[60] = "맞췄을 경우 보너스가 적용, 남은 횟수를 곱하여 계산합니다.";
	char Explain_score_4[60] = "만약, 정답을 맞춘 후, 남은 기회가 0번 이라면 보너스는";
	char Explain_score_5[60] = "적용되지 않으며 볼과 스트라이크의 합으로만 계산됩니다.";


	//설명 출력
	for (i = 0; i < 3; i++) {
		switch (i) {
		case 0: // 개요
			gotoxy(h - 2, y - 2);
			printf("1/3");
			gotoxy(h - 7, y - 1);
			printf("--게임 개요--\n");

			//Explain_summary
			for (i = 0; i < 6; i++) {
				switch (i) {
				case 0:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_summary_0[s]);
					}
					y++;
					break;
				case 1:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_summary_1[s]);
					}
					y++;
					break;
				case 2:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_summary_2[s]);
					}
					y++;
					break;
				case 3:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_summary_3[s]);
					}
					y++;
					break;
				case 4:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_summary_4[s]);
					}
					y++;
					break;
				case 5:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_summary_5[s]);
					}
					y++;
					break;
				default:
					break;
				}
			}
			_getch();
			y = 4;
			system("cls");

		case 1: // 게임 방법
			gotoxy(h - 2, y - 2);
			printf("2/3");
			gotoxy(h - 7, y - 1);
			printf("--게임 방법--\n");

			//Explain_way
			for (i = 0; i < 6; i++) {
				switch (i) {
				case 0:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_way_0[s]);
					}
					y++;
					break;
				case 1:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_way_1[s]);
					}
					y++;
					break;
				case 2:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_way_2[s]);
					}
					y++;
					break;
				case 3:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_way_3[s]);
					}
					y++;
					break;
				case 4:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_way_4[s]);
					}
					y++;
					break;
				case 5:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_way_5[s]);
					}
					y++;
					break;
				default:
					break;
				}
			}
			_getch();
			y = 4;
			system("cls");

		case 2: // 점수 계산
			gotoxy(h - 2, y - 2);
			printf("3/3");
			gotoxy(h - 7, y - 1);
			printf("--점수 계산--\n");

			//Explain_score
			for (i = 0; i < 6; i++) {
				switch (i) {
				case 0:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_score_0[s]);
					}
					y++;
					break;
				case 1:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_score_1[s]);
					}
					y++;
					break;
				case 2:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_score_2[s]);
					}
					y++;
					break;
				case 3:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_score_3[s]);
					}
					y++;
					break;
				case 4:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_score_4[s]);
					}
					y++;
					break;
				case 5:
					gotoxy(10, y);
					for (s = 0; s < 60; s++) {
						printf("%c", Explain_score_5[s]);
					}
					y++;
					break;
				default:
					break;
				}
			}
			_getch();
			y = 4;
			system("cls");
		}
	}
}

//숫자 야구 레벨 선택    ●   1 - 쉬움[3] / 2 - 보통[4] / 3 - 어려움[5] 
int Select_Level() {
	gotoxy(31, 5);
	printf("●○-숫자 야구-○●");
	gotoxy(32, 6);
	printf("--Select Level--");

	//레벨 보기
	for (j = 0; j < 4; j++) {
		gotoxy(31, j + 7);
		switch (j) {
		case 0:
			printf("[1]  쉬움 - 3자리");
			break;
		case 1:
			printf("[2]  보통 - 4자리");
			break;
		case 2:
			printf("[3] 어려움 - 5자리");
			break;
		case 3:
			gotoxy(31, j + 7);
			printf("==================");
			gotoxy(31, j + 8);
			printf("메뉴 선택 : ");
			break;
		}
	}

	//레벨 입력
	game_switch = _getch();
	game_switch = game_switch - '0';
	//scanf_s("%d", &game_switch);
	system("cls");
	return game_switch;
}

//숫자 입력 받기		●  
int Get_user_number(int x_pos, int y_pos) {
	int Input;
	gotoxy(x_pos, y_pos);
	scanf_s("%d", &Input);
	return Input;
}

//배열에 숫자 입력		●
void Input_user_number(int level, int input) {
	int mil, tsnd, hun, ten, one;


	switch (level)
	{
	case 1: //3자리
		hun = input / 100;
		input = input - (hun * 100);
		ten = input / 10;
		input = input - (ten * 10);
		one = input;

		User_number[turn_log][0] = Int_to_Char(hun);
		User_number[turn_log][1] = Int_to_Char(ten);
		User_number[turn_log][2] = Int_to_Char(one);

		break;
	case 2: //4자리
		//자릿수별 숫자
		tsnd = input / 1000;
		input = input - (tsnd * 1000);
		hun = input / 100;
		input = input - (hun * 100);
		ten = input / 10;
		input = input - (ten * 10);
		one = input;

		//문자형 저장
		User_number[turn_log][0] = Int_to_Char(tsnd);
		User_number[turn_log][1] = Int_to_Char(hun);
		User_number[turn_log][2] = Int_to_Char(ten);
		User_number[turn_log][3] = Int_to_Char(one);

		break;

	case 3: //5자리
		mil = input / 10000;
		input = input - (mil * 10000);
		tsnd = input / 1000;
		input = input - (tsnd * 1000);
		hun = input / 100;
		input = input - (hun * 100);
		ten = input / 10;
		input = input - (ten * 10);
		one = input;

		User_number[turn_log][0] = Int_to_Char(mil);
		User_number[turn_log][1] = Int_to_Char(tsnd);
		User_number[turn_log][2] = Int_to_Char(hun);
		User_number[turn_log][3] = Int_to_Char(ten);
		User_number[turn_log][4] = Int_to_Char(one);

		break;
	}
}

//입력숫자의 정답포함여부를 반환하는 함수
int Int_in_list(char x) {
	int i;
	char list[5] = { 0 };

	for (i = 0; i < level; i++)
		list[i] = Com_number[i];

	i = 0;
	do {
		if (list[i] == x)
			return TRUE;
		i++;
	} while (list[i] != NULL);

	return FALSE;
}

//게임 진행 상황	●
void Score_board() {
	gotoxy(0, 0);
	printf("----게임 상황---- \n");
	printf("현재 턴   : %d 턴\n", turn);
	printf("남은 횟수 : %d 번\n", chance);
	printf("현재 점수 : %d 점\n", score);
	printf("최고 점수 : %d 점\n", highest_score[game_switch - 1]);
	printf("=================\n");
	// (0,6) 아래로는 게임 상황을 표시.

}

//결과 띄우기		●
void Result(int turn, int S, int B, int O) {

	printf("[ %d 턴 ] \n", game_count[turn_log] + 1);

	for (i = 0; i < level; i++)
		printf("%c", User_number[turn_log][i]);
	printf(" ==> %d S  %d B  %d O\n", game_log[turn_log][1], game_log[turn_log][2], game_log[turn_log][3]);

}

//정답 판별		●
int Check_Correct() {
	if (Strike == level) {
		return TRUE;
		/*
		if (chn <= 1)
			return scr;
		else {
			score = score * chn;
			return score;
		}
		*/
	}
	else
		return FALSE;
}

//점수 보너스		●
int Bonus_score(int now_score, int left_chance) {
	if (left_chance <= 1)
		return now_score;
	else {
		now_score = now_score * left_chance;
		return now_score;
	}
}


//게임 리셋		●
void Reset() {
	//컴퓨터의 정답 숫자와 유저의 정답 숫자 초기화
	for (i = 0; i < 5; i++)
		Com_number[i] = 0;

	for (i = 0; i < 30; i++) {
		for (j = 0; j < 5; j++) {
			User_number[i][j] = 0;
		}
	}

	//유저의 입력 숫자 로그 초기화
	for (i = 0; i < 15; i++) {
		for (j = 0; j < 4; j++) {
			game_log[i][j] = 0;
		}
	}
	turn_log = 0;
	Number_log = 0;
	S_log = 0;
	B_log = 0;
	O_log = 0;
	get_num = 0;

	//중복 제거 초기화
	for (i = 0; i < 10; i++)
		no_overlap[i] = 0;

	//인덱스 접근 초기화
	idx = 0;

	//레벨, 찬스, 턴 초기화
	level = 0;
	chance = 0;
	turn = 1;

	// S, B, O 초기화
	Strike = 0;
	Ball = 0;
	Out = 0;

	//결과 창 출력제어 변수 초기화
		//게임 턴수 표시
	for (i = 0; i < 50; i++) {
		game_count[i] = i;
	}
	//출력 위치 표시
	result_pos = 6;

	//점수 초기화
	score = 0;


}

//최고점수 리셋		●
void Highest_Score_Reset() {
	system("cls");

	gotoxy(27, 7);
	printf("초기화 할 난이도 선택");
	gotoxy(30, 9);
	printf("[1] - 쉬움");
	gotoxy(30, 10);
	printf("[2] - 보통");
	gotoxy(30, 11);
	printf("[3] - 어려움");
	gotoxy(30, 12);
	printf("[4] - 전체 초기화");
	gotoxy(22, 14);
	printf("ESC를 누르면 메뉴로 돌아갑니다.");

	reset = _getch();
	system("cls");

	switch (reset)
	{
	case '1':
		gotoxy(16, 9);
		printf("정말로 쉬움 난이도의 최고 점수를 초기화 하시겠습니까?");
		gotoxy(26, 11);
		printf("1 - 초기화  /  2 - 취소\n");

		reset = _getch();

		if (reset == '1') {
			system("cls");
			highest_score[0] = 0;
			gotoxy(21, 9);
			printf("최고 점수가 0점 으로 초기화 되었습니다");
			Sleep(2500);
		}
		system("cls");
		break;
	case '2':
		gotoxy(16, 9);
		printf("정말로 보통 난이도의 최고 점수를 초기화 하시겠습니까?");
		gotoxy(26, 11);
		printf("1 - 초기화  /  2 - 취소\n");

		reset = _getch();

		if (reset == '1') {
			system("cls");
			highest_score[1] = 0;
			gotoxy(21, 9);
			printf("최고 점수가 0점 으로 초기화 되었습니다");
			Sleep(2500);
		}
		system("cls");
		break;

	case '3':
		gotoxy(12, 9);
		printf("정말로 어려움 난이도의 최고 점수를 초기화 하시겠습니까?");
		gotoxy(26, 11);
		printf("1 - 초기화  /  2 - 취소\n");

		reset = _getch();

		if (reset == '1') {
			system("cls");
			highest_score[2] = 0;
			gotoxy(21, 9);
			printf("최고 점수가 0점 으로 초기화 되었습니다");
			Sleep(2500);
		}
		system("cls");
		break;

	case '4':
		gotoxy(16, 9);
		printf("정말로 모든 난이도의 최고 점수를 초기화 하시겠습니까?");
		gotoxy(26, 11);
		printf("1 - 초기화  /  2 - 취소\n");

		reset = _getch();

		if (reset == '1') {
			system("cls");
			for (i = 0; i < 3; i++)
				highest_score[i] = 0;
			gotoxy(21, 9);
			printf("최고 점수가 0점 으로 초기화 되었습니다");
			Sleep(2500);
		}
		system("cls");
		break;

	default:
		break;
	}
}


//숫자 야구 게임 메인 함수		★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
void Number_Baseball(int selected_level) {

	int random_unit;	//랜덤숫자 생성

	// 숫자야구 실행
	srand((unsigned)time(0));
	switch (selected_level)
	{
	case 1: // 쉬움 - 3자리
		//초기화
		Reset();

		// 난이도 세팅
		level = 3;
		chance = 7;


		// 문제 생성
		while (1) {
			random_unit = rand() % 10;
			no_overlap[random_unit]++;
			//중복 제거
			if (no_overlap[random_unit] <= 1) {
				Com_number[idx] = Int_to_Char(random_unit);
				idx++;
			}
			//정답 숫자 생성
			if (idx == level)
				break;
		}

		// 게임 시작
		turn_log = 0;
		do {
			//현황판 출력

			//정답 확인
			/*
			gotoxy(70, 23);
			for (i=0;i<level;i++)
				printf("%c ", Com_number[i]);
			*/

			gotoxy(1, 1);
			Score_board();
			//턴별 결과
			switch (turn) {
			case 1:
				break;
			default:
				for (turn_log = 0; turn_log < turn - 1; turn_log++) {
					gotoxy(0, result_pos);
					Result(turn, game_log[turn_log][1], game_log[turn_log][2], game_log[turn_log][3]);
					result_pos += 4;
				}
				result_pos = 6;
				break;
			}

			x = 35;
			y = 8;

			//사용자 숫자 입력
			gotoxy(x, y);
			printf("숫자 %d자리 입력 : ", level); // 문자열 길이 : 18
			game_log[turn_log][0] = Get_user_number(x + 18, y);
			Input_user_number(selected_level, game_log[turn_log][0]);
			get_num++;


			//SBO 판단, 점수계산
			Strike = 0;
			Ball = 0;
			Out = 0;
			i = 0;
			do
			{
				if (Int_in_list(User_number[turn_log][i]) == TRUE) {
					//이 단계로 들어오면, 입력 숫자는 정답숫자에 포함이 되어 있으므로 자릿수를 비교하여 S와 B을 판단.

					//Strike
					if (User_number[turn_log][i] == Com_number[i]) {
						Strike++;
						game_log[turn_log][1] = Strike;
						score += 100;
						if (score > highest_score[0])
							highest_score[0] = score;
					}

					//Ball
					else if (User_number[turn_log][i] != Com_number[i]) {
						Ball++;
						game_log[turn_log][2] = Ball;
						score += 50;
						if (score > highest_score[0])
							highest_score[0] = score;
					}
				}

				//Out
				else if (Int_in_list(User_number[turn_log][i]) == FALSE)
				{
					Out++;
					game_log[turn_log][3] = Out;
				}

				i++;

			} while (User_number[turn_log][i] != NULL);


			//정답 판별
			if (Check_Correct() == TRUE) {
				score = Bonus_score(score, chance);
				if (score > highest_score[0])
					highest_score[0] = score;
				system("cls");
				break;
			}


			// 턴 마무리
			turn++;
			turn_log++;
			chance--;
			system("cls");
		} while (chance != 0);


		//종료 화면

		if (Check_Correct() == TRUE) {
			// 승리
			gotoxy(36, 5);
			printf("게임 승리");
			gotoxy(32, 6);
			printf("횟수 보너스 : X %d", chance);

		}
		else {
			//패배
			gotoxy(36, 5);
			printf("게임 패배");
		}

		gotoxy(33, 8);
		printf("정답 : [ ");
		for (i = 0; i < level; i++)
			printf("%c ", Com_number[i]);
		printf("]");
		gotoxy(26, 10);
		printf("게임이 종료되었습니다. (%d 점)", score);
		gotoxy(27, 12);
		printf("1 - 재시작  /  2 - 메뉴로\n");

		Reset();

		game_switch = _getch();

		// 1 - 재시작 / 2 - 메뉴
		switch (game_switch)
		{
		case '1':
			system("cls");
			Number_Baseball(Select_Level());
		default:
			break;
		}
		break;

	case 2: // 보통 - 4자리
		//초기화
		Reset();

		// 난이도 세팅
		level = 4;
		chance = 10;


		// 문제 생성
		while (1) {
			random_unit = rand() % 10;
			no_overlap[random_unit]++;
			//중복 제거
			if (no_overlap[random_unit] <= 1) {
				Com_number[idx] = Int_to_Char(random_unit);
				idx++;
			}
			//정답 숫자 생성
			if (idx == level)
				break;
		}

		// 게임 시작
		turn_log = 0;
		do {
			//현황판 출력

			//정답 확인
			/*
			gotoxy(70, 23);
			for (i = 0; i < level; i++)
				printf("%c ", Com_number[i]);
			*/


			gotoxy(1, 1);
			Score_board();
			//턴별 결과
			switch (turn) {
			case 1:
				break;
			default:
				for (turn_log = 0; turn_log < turn - 1; turn_log++) {
					gotoxy(0, result_pos);
					Result(turn, game_log[turn_log][1], game_log[turn_log][2], game_log[turn_log][3]);
					result_pos += 4;
				}
				result_pos = 6;
				break;
			}

			x = 35;
			y = 8;

			//사용자 숫자 입력
			gotoxy(x, y);
			printf("숫자 %d자리 입력 : ", level); // 문자열 길이 : 18
			game_log[turn_log][0] = Get_user_number(x + 18, y);
			Input_user_number(selected_level, game_log[turn_log][0]);
			get_num++;


			//SBO 판단, 점수계산
			Strike = 0;
			Ball = 0;
			Out = 0;
			i = 0;
			do
			{
				if (Int_in_list(User_number[turn_log][i]) == TRUE) {
					//이 단계로 들어오면, 입력 숫자는 정답숫자에 포함이 되어 있으므로 자릿수를 비교하여 S와 B을 판단.

					//Strike
					if (User_number[turn_log][i] == Com_number[i]) {
						Strike++;
						game_log[turn_log][1] = Strike;
						score += 100;
						if (score > highest_score[1])
							highest_score[1] = score;
					}

					//Ball
					else if (User_number[turn_log][i] != Com_number[i]) {
						Ball++;
						game_log[turn_log][2] = Ball;
						score += 50;
						if (score > highest_score[1])
							highest_score[1] = score;
					}
				}

				//Out
				else if (Int_in_list(User_number[turn_log][i]) == FALSE)
				{
					Out++;
					game_log[turn_log][3] = Out;
				}

				i++;

			} while (User_number[turn_log][i] != NULL);

			//정답 판별
			if (Check_Correct() == TRUE) {
				score = Bonus_score(score, chance);
				if (score > highest_score[1])
					highest_score[1] = score;
				system("cls");
				break;
			}

			// 턴 마무리
			turn++;
			turn_log++;
			chance--;
			system("cls");
		} while (chance != 0);


		//종료 화면
		if (Check_Correct() == TRUE) {
			// 승리
			gotoxy(36, 5);
			printf("게임 승리");
			gotoxy(32, 6);
			printf("횟수 보너스 : X %d", chance);

		}
		else {
			//패배
			gotoxy(36, 5);
			printf("게임 패배");
		}

		gotoxy(32, 8);
		printf("정답 : [ ");
		for (i = 0; i < level; i++)
			printf("%c ", Com_number[i]);
		printf("]");
		gotoxy(26, 10);
		printf("게임이 종료되었습니다. (%d 점)", score);
		gotoxy(27, 12);
		printf("1 - 재시작  /  2 - 메뉴로\n");

		Reset();

		game_switch = _getch();
		switch (game_switch)
		{
		case '1':
			system("cls");
			Number_Baseball(Select_Level());
		default:
			break;
		}
		break;

	case 3: // 어려움 - 5자리
		//초기화
		Reset();

		// 난이도 세팅
		level = 5;
		chance = 15;


		// 문제 생성
		while (1) {
			random_unit = rand() % 10;
			no_overlap[random_unit]++;
			//중복 제거
			if (no_overlap[random_unit] <= 1) {
				Com_number[idx] = Int_to_Char(random_unit);
				idx++;
			}
			//정답 숫자 생성
			if (idx == level)
				break;
		}

		// 게임 시작
		turn_log = 0;
		do {
			//현황판 출력

			//정답 확인
			/*
			gotoxy(70, 23);
			for (i=0;i<level;i++)
				printf("%c ", Com_number[i]);
			*/


			gotoxy(1, 1);
			Score_board();
			//턴별 결과
			switch (turn) {
			case 1:
				break;
			default:
				for (turn_log = 0; turn_log < turn - 1; turn_log++) {
					gotoxy(0, result_pos);
					Result(turn, game_log[turn_log][1], game_log[turn_log][2], game_log[turn_log][3]);
					result_pos += 4;
				}
				result_pos = 6;
				break;
			}

			x = 35;
			y = 8;

			//사용자 숫자 입력
			gotoxy(x, y);
			printf("숫자 %d자리 입력 : ", level); // 문자열 길이 : 18
			game_log[turn_log][0] = Get_user_number(x + 18, y);
			Input_user_number(selected_level, game_log[turn_log][0]);
			get_num++;


			//SBO 판단, 점수계산
			Strike = 0;
			Ball = 0;
			Out = 0;
			i = 0;
			do
			{
				if (Int_in_list(User_number[turn_log][i]) == TRUE) {
					//이 단계로 들어오면, 입력 숫자는 정답숫자에 포함이 되어 있으므로 자릿수를 비교하여 S와 B을 판단.

					//Strike
					if (User_number[turn_log][i] == Com_number[i]) {
						Strike++;
						game_log[turn_log][1] = Strike;
						score += 100;
						if (score > highest_score[2])
							highest_score[2] = score;
					}

					//Ball
					else if (User_number[turn_log][i] != Com_number[i]) {
						Ball++;
						game_log[turn_log][2] = Ball;
						score += 50;
						if (score > highest_score[2])
							highest_score[2] = score;
					}
				}

				//Out
				else if (Int_in_list(User_number[turn_log][i]) == FALSE)
				{
					Out++;
					game_log[turn_log][3] = Out;
				}

				i++;

			} while (User_number[turn_log][i] != NULL);

			//정답 판별
			if (Check_Correct() == TRUE) {
				score = Bonus_score(score, chance);
				if (score > highest_score[2])
					highest_score[2] = score;
				system("cls");
				break;
			}

			// 턴 마무리
			turn++;
			turn_log++;
			chance--;
			system("cls");
		} while (chance != 0);


		//종료 화면
		if (Check_Correct() == TRUE) {
			// 승리
			gotoxy(36, 5);
			printf("게임 승리");
			gotoxy(32, 6);
			printf("횟수 보너스 : X %d", chance);

		}
		else {
			//패배
			gotoxy(36, 5);
			printf("게임 패배");
		}

		gotoxy(30, 8);
		printf("정답 : [ ");
		for (i = 0; i < level; i++)
			printf("%c ", Com_number[i]);
		printf("]");
		gotoxy(26, 10);
		printf("게임이 종료되었습니다. (%d 점)", score);
		gotoxy(27, 12);
		printf("1 - 재시작  /  2 - 메뉴로\n");

		Reset();

		game_switch = _getch();
		switch (game_switch)
		{
		case '1':
			system("cls");
			Number_Baseball(Select_Level());
		default:
			break;
		}
		break;
	default:
		break;
	}

	system("cls");
}
//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

//게임 메뉴    ●
void Menu(int title) {
	switch (title) {
	case 1:		//게임 시작
		Number_Baseball(Select_Level());  //선택된 레벨(난이도)에 따라 각기 다른 숫자 야구 알고리즘을 실행.
		break;

	case 2:		//게임 방법
		Game_Explain();  //게임 설명  ●
		break;

	case 3:		//최고점수 확인
		system("cls");
		gotoxy(31, 8);
		printf(" 쉬움    : %d점", highest_score[0]);
		gotoxy(31, 9);
		printf(" 보통    : %d점", highest_score[1]);
		gotoxy(31, 10);
		printf("어려움   : %d점", highest_score[2]);
		gotoxy(30, 11);
		printf("====================");
		gotoxy(30, 12);
		printf("  점수 초기화 : C");
		gotoxy(30, 13);
		printf("  메뉴 닫기 : Esc");

		reset = _getch();
		system("cls");

		//최고 점수 초기화 여부 결정  (c,C => 초기화, else => 메뉴복귀)
		switch (reset) {
		case 'c': //c
		case 'C': //C
			Highest_Score_Reset();
		default:
			break;
		}
		break;

	case 4:		//게임 종료
		system("cls");
		gotoxy(27, 9);
		printf("게임을 종료 하시겠습니까?");
		gotoxy(28, 11);
		printf("1 - 종료  /  2 - 취소\n");

		game_switch = _getch();

		if (game_switch == '1') {
			game_switch = 4;
			break;
		}
		else
			system("cls");
	}
}


//메인 함수
int main(void) {
	int x = 0;

	do {
		game_switch = NULL;
		//메인화면에서 선택한 값을 Menu()에 인수로 넘기고, 인수를 받은 Menu()함수는 인수에 맞는 메뉴를 실행시킨다.
		Menu(Title());

	} while (game_switch != 4);
}
