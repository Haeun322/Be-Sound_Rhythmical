/**
#pragma once
#define _CRT_SECURE_NO_WARNNIGS
#pragma warning(disable : 4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <windows.h>
#include <mmsystem.h>
#include <process.h>
#include <time.h>
#include "gameplay.h"
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 13
#define ESC 27
#define score_file "score.txt"
#define MUSIC_NUMBER 4
#pragma comment(lib,"winmm.lib")
int score[MUSIC_NUMBER + 1];
int start = 0;

int restart = 0;

typedef struct score_information {
	char name[100];
	int bestscore;
}info;

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


void load_score() {
	info s[MUSIC_NUMBER];
	FILE* fp = fopen(score_file, "r");
	if (fp == NULL) {
		printf("fail to read file");
		return 0;
	}
	char buffer[1001], * token;
	int i = 0;
	int idx = 0;
	while (!feof(fp)) {
		i = 0;
		fgets(buffer, 1001, fp);
		token = strtok(buffer, " ");
		while (token != NULL) {
			if (i == 0) {
				strcpy(s[idx].name, token);
			}
			else if (i == 1) {
				s[idx].bestscore = atoi(token);
			}
			i++;
			token = strtok(NULL, " ");
		}
		idx++;
	}
	for (int i = 0; i < idx; i++) {
		printf("%s %d\n", s[i].name, s[i].bestscore);
	}
	fclose(fp);
	for (int i = 0; i < MUSIC_NUMBER; i++) {
		score[i] = s[i].bestscore;
	}
	return;
}


색 가이드
0 : 검정색
1 : 파란색
2 : 초록색
3 : 옥색
4 : 빨간색
5 : 자주색
6 : 노란색
7 : 흰색
8 : 회색
9 : 연한 파란색
10 : 연한 초록색
11 : 연한 옥색
12 : 연한 빨간색
13 : 연한 자주색
14 : 연한 노란색
15 : 진한 회색


아이콘
						  {{0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0} },

int lines[11][10][10] = {
						  {{0,0,0,0,0,0,0,0,0,0},
						   {0,12,12,0,0,0,0,12,12,0},
						   {12,12,12,12,0,0,12,12,12,12},
						   {12,12,12,12,12,12,12,12,12,12},
						   {12,12,12,12,12,12,12,12,12,12},
						   {0,12,12,12,12,12,12,12,12,0},
						   {0,0,12,12,12,12,12,12,0,0},
						   {0,0,0,12,12,12,12,0,0,0},
						   {0,0,0,0,12,12,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0} },

						  {{0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0},
						   {0,0,7,7,0,0,0,0,0,0},
						   {0,14,7,7,0,0,0,0,0,0},
						   {0,0,0,7,0,0,0,0,0,0},
						   {0,0,0,7,7,7,7,7,7,0},
						   {0,0,0,7,7,7,7,7,7,0},
						   {0,0,0,0,7,7,7,7,0,0},
						   {1,1,1,9,9,9,9,9,9,1},
						   {1,1,1,1,1,1,1,1,1,1} },

						 { {0,0,0,0,0,0,0,0,0,0},
						   {14,0,0,0,0,0,0,0,0,14},
						   {14,0,0,0,14,14,0,0,0,14},
						   {14,14,0,14,6,6,14,0,14,14},
						   {14,6,14,6,6,6,6,14,6,14},
						   {14,6,6,6,6,6,6,6,6,14},
						   {14,4,6,6,4,4,6,6,4,14},
						   {14,4,6,6,4,4,6,6,4,14},
						   {14,14,14,14,14,14,14,14,14,14},
						   {0,0,0,0,0,0,0,0,0,0} },

						 { {0,0,0,0,0,0,0,0,0,0},
						   {5,5,5,5,5,5,5,5,5,5},
						   {5,5,5,5,5,5,5,5,5,5},
						   {0,0,5,5,0,0,5,5,0,0},
						   {0,0,5,5,0,0,5,5,0,0},
						   {0,0,5,5,0,0,5,5,0,0},
						   {0,0,5,5,0,0,5,5,0,0},
						   {0,0,5,5,0,0,5,5,0,5},
						   {0,0,5,5,0,0,5,5,5,0},
						   {0,0,0,0,0,0,0,0,0,0} },

						 { {0,0,0,0,0,0,0,0,1,0},
						   {0,0,0,0,0,0,0,1,1,0},
						   {0,0,0,0,0,1,0,1,1,0},
						   {0,0,0,0,0,1,0,1,1,0},
						   {0,1,0,0,0,1,0,1,1,1},
						   {0,1,1,0,0,1,0,1,1,1},
						   {0,1,1,1,0,1,0,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1} },

						 { {0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0},
						   {0,1,0,0,0,0,0,0,1,0},
						   {0,1,0,0,0,0,0,1,1,0},
						   {0,1,0,1,0,1,0,1,1,0},
						   {0,1,0,1,0,1,1,1,1,0},
						   {0,1,1,1,1,1,1,1,1,0},
						   {0,1,1,1,1,1,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1} },

						 { {0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,1},
						   {0,0,1,0,0,0,0,0,0,1},
						   {0,0,1,0,0,0,0,0,0,1},
						   {1,0,1,1,0,0,0,1,1,1},
						   {1,0,1,1,0,1,0,1,1,1},
						   {1,0,1,1,0,1,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1} },

						 { {0,0,0,0,0,0,0,0,0,0},
						   {0,0,1,0,0,0,0,0,0,0},
						   {0,0,1,0,0,0,0,0,0,1},
						   {0,0,1,0,1,0,0,1,0,1},
						   {0,1,1,0,1,0,0,1,0,1},
						   {0,1,1,0,1,0,1,1,0,1},
						   {1,1,1,1,1,0,1,1,0,1},
						   {1,1,1,1,1,1,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1} },

						 { {0,0,1,0,0,0,0,0,0,0},
						   {0,0,1,0,0,0,0,0,0,0},
						   {0,0,1,0,1,0,0,1,0,0},
						   {1,0,1,0,1,0,1,1,0,1},
						   {1,0,1,1,1,0,1,1,1,1},
						   {1,0,1,1,1,1,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1} },

						 { {0,0,0,0,1,0,0,0,0,0},
						   {0,0,0,0,1,0,0,1,1,0},
						   {0,0,0,0,1,0,0,1,1,0},
						   {0,1,0,0,1,0,0,1,1,0},
						   {0,1,0,0,1,0,0,1,1,0},
						   {1,1,1,0,1,0,0,1,1,0},
						   {1,1,1,0,1,0,0,1,1,1},
						   {1,1,1,1,1,0,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1} },

						 { {0,0,0,0,0,0,0,1,0,0},
						   {0,0,0,0,0,0,0,1,0,0},
						   {0,1,0,1,0,0,0,1,0,0},
						   {0,1,0,1,0,0,0,1,0,1},
						   {0,1,0,1,1,0,1,1,0,1},
						   {0,1,0,1,1,0,1,1,1,1},
						   {0,1,0,1,1,0,1,1,1,1},
						   {1,1,1,1,1,0,1,1,1,1},
						   {1,1,1,1,1,0,1,1,1,1},
						   {1,1,1,1,1,1,1,1,1,1} }

};

void ClearCursor() {
	CONSOLE_CURSOR_INFO c = { 0 };
	c.dwSize = 1;
	c.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
}

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int setColor(int i, int j) {
	int Color = lines[music][i][j];
	return Color;
}

void draw_icon()
{
	int i = 0, j = 0, z = 0;
	int position = 4;
	int position2 = 4;
	
	position = 4;
	position2 = 4;
	for (i = 0; i < 10; i++) {
		gotoxy(52, position2);
		for (j = 0; j < 10; j++) {
			printf("  ");
		}
		printf("\n");
		position2++;
	}
	for (i = 0; i < 10; i++) {
		gotoxy(52, position);
		for (j = 0; j < 10; j++) {
			if (lines[music][i][j] != 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), setColor(i, j));
				printf("■");
			}
			else
				printf("  ");
		}
		printf("\n");
		position += 1;
	}
	Sleep(50);
}

unsigned _stdcall input_key(void* ard)
{
	char key;
	while (1) {
		if (_kbhit()) {
			key = _getch();
			switch (key) {
			case LEFT:
				PlaySound(TEXT("MP_Shoot Arrow.wav"), NULL, SND_ASYNC || SND_LOOP);
				if (music != 0)
					music--;
				play_music();
				draw_icon();
				break;
			case RIGHT:
				PlaySound(TEXT("MP_Shoot Arrow.wav"), NULL, SND_ASYNC || SND_LOOP);
				if (music != 2)
					music++;
				play_music();
				draw_icon();
				break;
			case ESC:
				start = 0;
				break;
			}
		}
	}
}

void print_menu() {
	ClearCursor();
	gotoxy(50, 3);
	printf("┌─────────────────────┐");
	gotoxy(50, 4);
	printf("│                     │");
	gotoxy(50, 5);
	printf("│                     │");
	gotoxy(50, 6);
	printf("│                     │");
	gotoxy(50, 7);
	printf("│                     │");
	gotoxy(50, 8);
	printf("│                     │");
	gotoxy(50, 9);
	printf("│                     │");
	gotoxy(50, 10);
	printf("│                     │");
	gotoxy(50, 11);
	printf("│                     │");
	gotoxy(50, 12);
	printf("│                     │");
	gotoxy(50, 13);
	printf("│                     │");
	gotoxy(50, 14);
	printf("└─────────────────────┘");
	gotoxy(50, 19);
	printf("┌─────────────────────┐");
	gotoxy(56, 18);
	printf("♬ Music ♬");
	gotoxy(50, 20);
	printf("│                     │");
	gotoxy(50, 21);
	printf("└─────────────────────┘");
	gotoxy(54, 23);
	printf("★ Best Score ★");
	gotoxy(50, 24);
	printf("┌─────────────────────┐");
	gotoxy(50, 25);
	printf("│                     │");
	gotoxy(50, 26);
	printf("└─────────────────────┘");

	gotoxy(38, 28);
	printf("♪~♬ ♪♬~♪ ♪~♬ ♪♬~♪ ♪~♬ ♪♬~♪ ♪~♬ ♪");

}
void GoodBye() {
	system("cls");
	gotoxy(50, 15);
	printf("See You Next Time!!\n\n\n\n\n\n\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	PlaySound(TEXT("close.wav"), NULL, SND_ASYNC || SND_LOOP);
	Sleep(3000);
	exit(0);
}
void print_title() {
	//system("cls");
	char key;
	ClearCursor();
	char maker[100] = { 'M','a','d','e',' ','b','y',' ','H','a','e','u','n',' ','a','n','d',' ','H','a','n',' ','G','y','e','o','l' };
	int i = 0, flag = 0;
	gotoxy(53, 2);
	printf("Project Title");
	gotoxy(47, 25);
	while (maker) {
		printf("%c", maker[i]);
		i++;
		if (maker[i] == NULL)
			break;
	}
	gotoxy(57, 12);
	printf("Start");
	gotoxy(58, 14);
	printf("OUT");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

	gotoxy(54, 12);
	printf("▷");
	while (1) {
		if (_kbhit()) {
			key = _getch();
			switch (key) {
			case UP:
				gotoxy(54, 14);
				printf("  ");
				gotoxy(54, 12);
				printf("▷");
				PlaySound(TEXT("MP_Shoot Arrow.wav"), NULL, SND_ASYNC || SND_LOOP);
				flag = 0;
				break;
			case DOWN:
				gotoxy(54, 12);
				printf("  ");
				gotoxy(54, 14);
				printf("▷");
				PlaySound(TEXT("MP_Shoot Arrow.wav"), NULL, SND_ASYNC || SND_LOOP);
				flag = 1;
				break;
			case ENTER:
				if (flag == 0) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					return;
				}
				else if (flag == 1)
					GoodBye();
			}
		}
	}
}

void loading() {
	system("cls");

	PlaySound(TEXT("Applause.wav"), NULL, SND_ASYNC || SND_LOOP);
	int load = 10;
	gotoxy(44, 15);
	for (int i = 0; i < 9; i++)
		printf("□  ");
	Sleep(100);
	gotoxy(44, 15);
	int i = 1;
	while (i <= 9) {
		load += 10;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		gotoxy(58, 20);
		printf("%d%c", load, '%');
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
		printf("    ");
		gotoxy(40 + i * 4, 15);
		printf("♬");
		i++;

		Sleep(500);
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	PlaySound(NULL, 0, 0);
	start = 1;
}
**/