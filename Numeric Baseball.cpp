//=================================================================< ��� >===============================================================================
/*
���α׷� ��		: ���� �߱� 
���� ���		: 201907029 ������
���� ������		: 2019 - 12 - 06

���  :
	�� ���α׷��� Microsoft Visual Studio 2017�� ������� �ڵ� �Ǿ����ϴ�.
	This program is cording based on Microsoft Visual Studio 2017.

��ó :	X  (���� �ð��� ��� �����鸸 ����Ͽ� �����Ͽ����ϴ�. ���� �Ϻ� ������ ���ͳ��� ���� �������� �˰����� �����Ͽ� ���� ��������ϴ�.)
*/
//========================================================================================================================================================



// ��� ����
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<string.h>
#include<Windows.h>
//---------------------------------------------��������---------------------------------------------------------

int Max_Console_Width = 80, Max_Console_Height = 25;  //�ܼ� ������ ����
int i, j;	//�ݺ��� ���� ����
int x, y;	//��ǥ ���� ����
int idx = 0;	//�ε��� ���� ���� (idx)
int game_switch, reset;  //�޴� ���� ����ġ


int level;	//���� ���̵� (�ڸ���)
char Com_number[5] = { 0 };	//���� ����
char no_overlap[10] = { 0 };	//�ߺ�����
char User_number[30][5] = { 0 };	// [n ��°] [����� ����]
int get_num = 0;		//�Է�

//�Է� �α� ( [����], [Number, S, B, O] )
int game_log[15][4] = { 0 };
int turn_log = 0;
int Number_log = 0;
int S_log = 0;
int B_log = 0;
int O_log = 0;

int score = 0;						//���� ����
int highest_score[3] = { 0 };		//�ְ� ���� (0 - ���� / 1 - ���� / 2 - �����)


int Strike = 0, Ball = 0, Out = 0;  //��Ʈ����ũ, ��, �ƿ�
int chance, turn;	// ���� ��ȸ, ���� �ϼ�
int game_count[50];		//���â �� ǥ��
int result_pos = 6;		//���â ��� ��ġ

//------------------------------------------------��������----------------------------------------------------






//Ŀ�� ��ǥ �̵�	��
void gotoxy(int x, int y)
{
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//��ũ�� ���� Ŭ����
void cls_area(int x, int y, int a, int b) { //        (x,y => ���� ��ǥ / a,b => ���� ��ǥ)
	int i, j;
	gotoxy(x, y);
	for (i = 0; i < (b - y); i++) {
		for (j = 0; j < (a - x); j++) {
			gotoxy(x + j, y + i);
			printf(" ");
		}
	}
	// a - x : Ŭ���� ���� x ����
	// b - y : Ŭ���� ���� y ����
}

// ( Int --> Char ) ��ȯ �Լ�
char Int_to_Char(int x) {
	return x + '0';
}

//���� ����ȭ��    ��    ����ȭ��, �޴�����
int Title() {
	//�޴� ������
	gotoxy(31, 5);
	printf("�ܡ�-���� �߱�-�ۡ�");
	gotoxy(33, 6);
	printf("--Select Menu--");

	//�޴� ����Ʈ
	for (j = 0; j < 5; j++) {
		gotoxy(32, j + 7);
		switch (j) {
		case 0:
			printf("1. ���� ����");
			break;
		case 1:
			printf("2. ���� ���");
			break;
		case 2:
			printf("3. �ְ� ���� Ȯ��");
			break;
		case 3:
			printf("4. ���� ����");
			break;
		case 4:
			printf("================");
			gotoxy(32, 12);
			printf("�޴� ���� : ");
			break;
		}
	}

	//�޴� �Է�
	game_switch = _getch();
	game_switch = game_switch - '0';
	//scanf_s("%d", &game_switch);
	system("cls");
	return game_switch;
}

//���� ����    ��
void Game_Explain() {
	int s = 0; //���ڿ� �о���� ����
	int e = 8, h = 40, y = 4; //��¹� ��ġ ����

	//����
	char Explain_summary_0[60] = "���� �߱��� ������ ���ڸ� ���� �ƿ�, ��Ʈ����ũ�� ���Ͽ� ��";
	char Explain_summary_1[60] = "���� ��ȸ �ȿ� ���ߴ� �߸� �����Դϴ�. ����ڴ� �ڽ��� ����";
	char Explain_summary_2[60] = "�� ������ �� ���� �ڸ����� ������ �� ������, ������ �ڸ� ��";
	char Explain_summary_3[60] = "�� ���� ���� �� �ִ� ��ȸ�� �ٸ��� �־����ϴ�.�־��� ��ȸ";
	char Explain_summary_4[60] = "�ȿ��� ���� �ƿ�, ��Ʈ����ũ�� �̿��Ͽ� �߸��ϰ�, ������";
	char Explain_summary_5[60] = "���ڸ� ���߸� �¸��ϰ� �˴ϴ�.";

	//���� ���
	char Explain_way_0[60] = "�޴��� ���Ͽ� ������ �� �ڸ����� �Է� ������, ������ �ڸ���";
	char Explain_way_1[60] = "�� ���� ���ڸ� �Է��� �� �ְ� �˴ϴ�. �Է��� ���ڰ� ���";
	char Explain_way_2[60] = "���� ���ڿ� ���� ���� ���� \"�ƿ�\"���� ��Ÿ���ϴ�. �Է���";
	char Explain_way_3[60] = "���ڿ� ���� ������ ���ڴ� ������, �ڸ����� ���� ���� ����";
	char Explain_way_4[60] = "\"��\"�� ��Ÿ���ϴ�. �Է��� ���ڰ� ���� ������ ���ڿ� �ڸ���";
	char Explain_way_5[60] = "��� �����ϸ� \"��Ʈ����ũ\"�Դϴ�.";

	//���� ���
	char Explain_score_0[60] = "���� �߱��� �߸� ����� ��������, ������ ��Ȯ�ϰ� ���� ã��";
	char Explain_score_1[60] = "���� �߿��մϴ�. �ƿ��� 0�� �̸�, \"��\"�� �������� 50��, \"��";
	char Explain_score_2[60] = "Ʈ����ũ\"�� 100������ ���˴ϴ�. ������ Ƚ������ �̸���";
	char Explain_score_3[60] = "������ ��� ���ʽ��� ����, ���� Ƚ���� ���Ͽ� ����մϴ�.";
	char Explain_score_4[60] = "����, ������ ���� ��, ���� ��ȸ�� 0�� �̶�� ���ʽ���";
	char Explain_score_5[60] = "������� ������ ���� ��Ʈ����ũ�� �����θ� ���˴ϴ�.";


	//���� ���
	for (i = 0; i < 3; i++) {
		switch (i) {
		case 0: // ����
			gotoxy(h - 2, y - 2);
			printf("1/3");
			gotoxy(h - 7, y - 1);
			printf("--���� ����--\n");

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

		case 1: // ���� ���
			gotoxy(h - 2, y - 2);
			printf("2/3");
			gotoxy(h - 7, y - 1);
			printf("--���� ���--\n");

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

		case 2: // ���� ���
			gotoxy(h - 2, y - 2);
			printf("3/3");
			gotoxy(h - 7, y - 1);
			printf("--���� ���--\n");

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

//���� �߱� ���� ����    ��   1 - ����[3] / 2 - ����[4] / 3 - �����[5] 
int Select_Level() {
	gotoxy(31, 5);
	printf("�ܡ�-���� �߱�-�ۡ�");
	gotoxy(32, 6);
	printf("--Select Level--");

	//���� ����
	for (j = 0; j < 4; j++) {
		gotoxy(31, j + 7);
		switch (j) {
		case 0:
			printf("[1]  ���� - 3�ڸ�");
			break;
		case 1:
			printf("[2]  ���� - 4�ڸ�");
			break;
		case 2:
			printf("[3] ����� - 5�ڸ�");
			break;
		case 3:
			gotoxy(31, j + 7);
			printf("==================");
			gotoxy(31, j + 8);
			printf("�޴� ���� : ");
			break;
		}
	}

	//���� �Է�
	game_switch = _getch();
	game_switch = game_switch - '0';
	//scanf_s("%d", &game_switch);
	system("cls");
	return game_switch;
}

//���� �Է� �ޱ�		��  
int Get_user_number(int x_pos, int y_pos) {
	int Input;
	gotoxy(x_pos, y_pos);
	scanf_s("%d", &Input);
	return Input;
}

//�迭�� ���� �Է�		��
void Input_user_number(int level, int input) {
	int mil, tsnd, hun, ten, one;


	switch (level)
	{
	case 1: //3�ڸ�
		hun = input / 100;
		input = input - (hun * 100);
		ten = input / 10;
		input = input - (ten * 10);
		one = input;

		User_number[turn_log][0] = Int_to_Char(hun);
		User_number[turn_log][1] = Int_to_Char(ten);
		User_number[turn_log][2] = Int_to_Char(one);

		break;
	case 2: //4�ڸ�
		//�ڸ����� ����
		tsnd = input / 1000;
		input = input - (tsnd * 1000);
		hun = input / 100;
		input = input - (hun * 100);
		ten = input / 10;
		input = input - (ten * 10);
		one = input;

		//������ ����
		User_number[turn_log][0] = Int_to_Char(tsnd);
		User_number[turn_log][1] = Int_to_Char(hun);
		User_number[turn_log][2] = Int_to_Char(ten);
		User_number[turn_log][3] = Int_to_Char(one);

		break;

	case 3: //5�ڸ�
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

//�Է¼����� �������Կ��θ� ��ȯ�ϴ� �Լ�
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

//���� ���� ��Ȳ	��
void Score_board() {
	gotoxy(0, 0);
	printf("----���� ��Ȳ---- \n");
	printf("���� ��   : %d ��\n", turn);
	printf("���� Ƚ�� : %d ��\n", chance);
	printf("���� ���� : %d ��\n", score);
	printf("�ְ� ���� : %d ��\n", highest_score[game_switch - 1]);
	printf("=================\n");
	// (0,6) �Ʒ��δ� ���� ��Ȳ�� ǥ��.

}

//��� ����		��
void Result(int turn, int S, int B, int O) {

	printf("[ %d �� ] \n", game_count[turn_log] + 1);

	for (i = 0; i < level; i++)
		printf("%c", User_number[turn_log][i]);
	printf(" ==> %d S  %d B  %d O\n", game_log[turn_log][1], game_log[turn_log][2], game_log[turn_log][3]);

}

//���� �Ǻ�		��
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

//���� ���ʽ�		��
int Bonus_score(int now_score, int left_chance) {
	if (left_chance <= 1)
		return now_score;
	else {
		now_score = now_score * left_chance;
		return now_score;
	}
}


//���� ����		��
void Reset() {
	//��ǻ���� ���� ���ڿ� ������ ���� ���� �ʱ�ȭ
	for (i = 0; i < 5; i++)
		Com_number[i] = 0;

	for (i = 0; i < 30; i++) {
		for (j = 0; j < 5; j++) {
			User_number[i][j] = 0;
		}
	}

	//������ �Է� ���� �α� �ʱ�ȭ
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

	//�ߺ� ���� �ʱ�ȭ
	for (i = 0; i < 10; i++)
		no_overlap[i] = 0;

	//�ε��� ���� �ʱ�ȭ
	idx = 0;

	//����, ����, �� �ʱ�ȭ
	level = 0;
	chance = 0;
	turn = 1;

	// S, B, O �ʱ�ȭ
	Strike = 0;
	Ball = 0;
	Out = 0;

	//��� â ������� ���� �ʱ�ȭ
		//���� �ϼ� ǥ��
	for (i = 0; i < 50; i++) {
		game_count[i] = i;
	}
	//��� ��ġ ǥ��
	result_pos = 6;

	//���� �ʱ�ȭ
	score = 0;


}

//�ְ����� ����		��
void Highest_Score_Reset() {
	system("cls");

	gotoxy(27, 7);
	printf("�ʱ�ȭ �� ���̵� ����");
	gotoxy(30, 9);
	printf("[1] - ����");
	gotoxy(30, 10);
	printf("[2] - ����");
	gotoxy(30, 11);
	printf("[3] - �����");
	gotoxy(30, 12);
	printf("[4] - ��ü �ʱ�ȭ");
	gotoxy(22, 14);
	printf("ESC�� ������ �޴��� ���ư��ϴ�.");

	reset = _getch();
	system("cls");

	switch (reset)
	{
	case '1':
		gotoxy(16, 9);
		printf("������ ���� ���̵��� �ְ� ������ �ʱ�ȭ �Ͻðڽ��ϱ�?");
		gotoxy(26, 11);
		printf("1 - �ʱ�ȭ  /  2 - ���\n");

		reset = _getch();

		if (reset == '1') {
			system("cls");
			highest_score[0] = 0;
			gotoxy(21, 9);
			printf("�ְ� ������ 0�� ���� �ʱ�ȭ �Ǿ����ϴ�");
			Sleep(2500);
		}
		system("cls");
		break;
	case '2':
		gotoxy(16, 9);
		printf("������ ���� ���̵��� �ְ� ������ �ʱ�ȭ �Ͻðڽ��ϱ�?");
		gotoxy(26, 11);
		printf("1 - �ʱ�ȭ  /  2 - ���\n");

		reset = _getch();

		if (reset == '1') {
			system("cls");
			highest_score[1] = 0;
			gotoxy(21, 9);
			printf("�ְ� ������ 0�� ���� �ʱ�ȭ �Ǿ����ϴ�");
			Sleep(2500);
		}
		system("cls");
		break;

	case '3':
		gotoxy(12, 9);
		printf("������ ����� ���̵��� �ְ� ������ �ʱ�ȭ �Ͻðڽ��ϱ�?");
		gotoxy(26, 11);
		printf("1 - �ʱ�ȭ  /  2 - ���\n");

		reset = _getch();

		if (reset == '1') {
			system("cls");
			highest_score[2] = 0;
			gotoxy(21, 9);
			printf("�ְ� ������ 0�� ���� �ʱ�ȭ �Ǿ����ϴ�");
			Sleep(2500);
		}
		system("cls");
		break;

	case '4':
		gotoxy(16, 9);
		printf("������ ��� ���̵��� �ְ� ������ �ʱ�ȭ �Ͻðڽ��ϱ�?");
		gotoxy(26, 11);
		printf("1 - �ʱ�ȭ  /  2 - ���\n");

		reset = _getch();

		if (reset == '1') {
			system("cls");
			for (i = 0; i < 3; i++)
				highest_score[i] = 0;
			gotoxy(21, 9);
			printf("�ְ� ������ 0�� ���� �ʱ�ȭ �Ǿ����ϴ�");
			Sleep(2500);
		}
		system("cls");
		break;

	default:
		break;
	}
}


//���� �߱� ���� ���� �Լ�		�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�
void Number_Baseball(int selected_level) {

	int random_unit;	//�������� ����

	// ���ھ߱� ����
	srand((unsigned)time(0));
	switch (selected_level)
	{
	case 1: // ���� - 3�ڸ�
		//�ʱ�ȭ
		Reset();

		// ���̵� ����
		level = 3;
		chance = 7;


		// ���� ����
		while (1) {
			random_unit = rand() % 10;
			no_overlap[random_unit]++;
			//�ߺ� ����
			if (no_overlap[random_unit] <= 1) {
				Com_number[idx] = Int_to_Char(random_unit);
				idx++;
			}
			//���� ���� ����
			if (idx == level)
				break;
		}

		// ���� ����
		turn_log = 0;
		do {
			//��Ȳ�� ���

			//���� Ȯ��
			/*
			gotoxy(70, 23);
			for (i=0;i<level;i++)
				printf("%c ", Com_number[i]);
			*/

			gotoxy(1, 1);
			Score_board();
			//�Ϻ� ���
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

			//����� ���� �Է�
			gotoxy(x, y);
			printf("���� %d�ڸ� �Է� : ", level); // ���ڿ� ���� : 18
			game_log[turn_log][0] = Get_user_number(x + 18, y);
			Input_user_number(selected_level, game_log[turn_log][0]);
			get_num++;


			//SBO �Ǵ�, �������
			Strike = 0;
			Ball = 0;
			Out = 0;
			i = 0;
			do
			{
				if (Int_in_list(User_number[turn_log][i]) == TRUE) {
					//�� �ܰ�� ������, �Է� ���ڴ� ������ڿ� ������ �Ǿ� �����Ƿ� �ڸ����� ���Ͽ� S�� B�� �Ǵ�.

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


			//���� �Ǻ�
			if (Check_Correct() == TRUE) {
				score = Bonus_score(score, chance);
				if (score > highest_score[0])
					highest_score[0] = score;
				system("cls");
				break;
			}


			// �� ������
			turn++;
			turn_log++;
			chance--;
			system("cls");
		} while (chance != 0);


		//���� ȭ��

		if (Check_Correct() == TRUE) {
			// �¸�
			gotoxy(36, 5);
			printf("���� �¸�");
			gotoxy(32, 6);
			printf("Ƚ�� ���ʽ� : X %d", chance);

		}
		else {
			//�й�
			gotoxy(36, 5);
			printf("���� �й�");
		}

		gotoxy(33, 8);
		printf("���� : [ ");
		for (i = 0; i < level; i++)
			printf("%c ", Com_number[i]);
		printf("]");
		gotoxy(26, 10);
		printf("������ ����Ǿ����ϴ�. (%d ��)", score);
		gotoxy(27, 12);
		printf("1 - �����  /  2 - �޴���\n");

		Reset();

		game_switch = _getch();

		// 1 - ����� / 2 - �޴�
		switch (game_switch)
		{
		case '1':
			system("cls");
			Number_Baseball(Select_Level());
		default:
			break;
		}
		break;

	case 2: // ���� - 4�ڸ�
		//�ʱ�ȭ
		Reset();

		// ���̵� ����
		level = 4;
		chance = 10;


		// ���� ����
		while (1) {
			random_unit = rand() % 10;
			no_overlap[random_unit]++;
			//�ߺ� ����
			if (no_overlap[random_unit] <= 1) {
				Com_number[idx] = Int_to_Char(random_unit);
				idx++;
			}
			//���� ���� ����
			if (idx == level)
				break;
		}

		// ���� ����
		turn_log = 0;
		do {
			//��Ȳ�� ���

			//���� Ȯ��
			/*
			gotoxy(70, 23);
			for (i = 0; i < level; i++)
				printf("%c ", Com_number[i]);
			*/


			gotoxy(1, 1);
			Score_board();
			//�Ϻ� ���
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

			//����� ���� �Է�
			gotoxy(x, y);
			printf("���� %d�ڸ� �Է� : ", level); // ���ڿ� ���� : 18
			game_log[turn_log][0] = Get_user_number(x + 18, y);
			Input_user_number(selected_level, game_log[turn_log][0]);
			get_num++;


			//SBO �Ǵ�, �������
			Strike = 0;
			Ball = 0;
			Out = 0;
			i = 0;
			do
			{
				if (Int_in_list(User_number[turn_log][i]) == TRUE) {
					//�� �ܰ�� ������, �Է� ���ڴ� ������ڿ� ������ �Ǿ� �����Ƿ� �ڸ����� ���Ͽ� S�� B�� �Ǵ�.

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

			//���� �Ǻ�
			if (Check_Correct() == TRUE) {
				score = Bonus_score(score, chance);
				if (score > highest_score[1])
					highest_score[1] = score;
				system("cls");
				break;
			}

			// �� ������
			turn++;
			turn_log++;
			chance--;
			system("cls");
		} while (chance != 0);


		//���� ȭ��
		if (Check_Correct() == TRUE) {
			// �¸�
			gotoxy(36, 5);
			printf("���� �¸�");
			gotoxy(32, 6);
			printf("Ƚ�� ���ʽ� : X %d", chance);

		}
		else {
			//�й�
			gotoxy(36, 5);
			printf("���� �й�");
		}

		gotoxy(32, 8);
		printf("���� : [ ");
		for (i = 0; i < level; i++)
			printf("%c ", Com_number[i]);
		printf("]");
		gotoxy(26, 10);
		printf("������ ����Ǿ����ϴ�. (%d ��)", score);
		gotoxy(27, 12);
		printf("1 - �����  /  2 - �޴���\n");

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

	case 3: // ����� - 5�ڸ�
		//�ʱ�ȭ
		Reset();

		// ���̵� ����
		level = 5;
		chance = 15;


		// ���� ����
		while (1) {
			random_unit = rand() % 10;
			no_overlap[random_unit]++;
			//�ߺ� ����
			if (no_overlap[random_unit] <= 1) {
				Com_number[idx] = Int_to_Char(random_unit);
				idx++;
			}
			//���� ���� ����
			if (idx == level)
				break;
		}

		// ���� ����
		turn_log = 0;
		do {
			//��Ȳ�� ���

			//���� Ȯ��
			/*
			gotoxy(70, 23);
			for (i=0;i<level;i++)
				printf("%c ", Com_number[i]);
			*/


			gotoxy(1, 1);
			Score_board();
			//�Ϻ� ���
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

			//����� ���� �Է�
			gotoxy(x, y);
			printf("���� %d�ڸ� �Է� : ", level); // ���ڿ� ���� : 18
			game_log[turn_log][0] = Get_user_number(x + 18, y);
			Input_user_number(selected_level, game_log[turn_log][0]);
			get_num++;


			//SBO �Ǵ�, �������
			Strike = 0;
			Ball = 0;
			Out = 0;
			i = 0;
			do
			{
				if (Int_in_list(User_number[turn_log][i]) == TRUE) {
					//�� �ܰ�� ������, �Է� ���ڴ� ������ڿ� ������ �Ǿ� �����Ƿ� �ڸ����� ���Ͽ� S�� B�� �Ǵ�.

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

			//���� �Ǻ�
			if (Check_Correct() == TRUE) {
				score = Bonus_score(score, chance);
				if (score > highest_score[2])
					highest_score[2] = score;
				system("cls");
				break;
			}

			// �� ������
			turn++;
			turn_log++;
			chance--;
			system("cls");
		} while (chance != 0);


		//���� ȭ��
		if (Check_Correct() == TRUE) {
			// �¸�
			gotoxy(36, 5);
			printf("���� �¸�");
			gotoxy(32, 6);
			printf("Ƚ�� ���ʽ� : X %d", chance);

		}
		else {
			//�й�
			gotoxy(36, 5);
			printf("���� �й�");
		}

		gotoxy(30, 8);
		printf("���� : [ ");
		for (i = 0; i < level; i++)
			printf("%c ", Com_number[i]);
		printf("]");
		gotoxy(26, 10);
		printf("������ ����Ǿ����ϴ�. (%d ��)", score);
		gotoxy(27, 12);
		printf("1 - �����  /  2 - �޴���\n");

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
//�ڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡڡ�

//���� �޴�    ��
void Menu(int title) {
	switch (title) {
	case 1:		//���� ����
		Number_Baseball(Select_Level());  //���õ� ����(���̵�)�� ���� ���� �ٸ� ���� �߱� �˰����� ����.
		break;

	case 2:		//���� ���
		Game_Explain();  //���� ����  ��
		break;

	case 3:		//�ְ����� Ȯ��
		system("cls");
		gotoxy(31, 8);
		printf(" ����    : %d��", highest_score[0]);
		gotoxy(31, 9);
		printf(" ����    : %d��", highest_score[1]);
		gotoxy(31, 10);
		printf("�����   : %d��", highest_score[2]);
		gotoxy(30, 11);
		printf("====================");
		gotoxy(30, 12);
		printf("  ���� �ʱ�ȭ : C");
		gotoxy(30, 13);
		printf("  �޴� �ݱ� : Esc");

		reset = _getch();
		system("cls");

		//�ְ� ���� �ʱ�ȭ ���� ����  (c,C => �ʱ�ȭ, else => �޴�����)
		switch (reset) {
		case 'c': //c
		case 'C': //C
			Highest_Score_Reset();
		default:
			break;
		}
		break;

	case 4:		//���� ����
		system("cls");
		gotoxy(27, 9);
		printf("������ ���� �Ͻðڽ��ϱ�?");
		gotoxy(28, 11);
		printf("1 - ����  /  2 - ���\n");

		game_switch = _getch();

		if (game_switch == '1') {
			game_switch = 4;
			break;
		}
		else
			system("cls");
	}
}


//���� �Լ�
int main(void) {
	int x = 0;

	do {
		game_switch = NULL;
		//����ȭ�鿡�� ������ ���� Menu()�� �μ��� �ѱ��, �μ��� ���� Menu()�Լ��� �μ��� �´� �޴��� �����Ų��.
		Menu(Title());

	} while (game_switch != 4);
}