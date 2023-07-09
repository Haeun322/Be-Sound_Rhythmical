/**
������� �� : gameplay
��� : ���� �÷��̿� ���Ǵ� ��� �Լ��� ������ ���� (���� ����, ����, �����, �ʱ�ȭ, ���� ��� ���)
**/
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"winmm.lib")
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <mmsystem.h>
#include <time.h>
#include <math.h>
#include <process.h>  
#include <conio.h>
#include "readmap.h"
#include "resource.h"
#include "Digitalv.h"
#include "calscore.h"
#include "menu.h"
#include "ImageLayer.h"
int music = 0;
int background = 0;
#pragma comment(lib,"Winmm.lib")
#define TRUE 1
#define FALSE 0
#define OEM 2
#define MAX_MUSIC 7
// �Է� ���� Ű ����
#define KEY_A 0x41
#define KEY_B 0x53
#define KEY_C 0x4C
#define KEY_D 0xBA
#define UP 0x72
#define DOWN 0x80
#define ENTER 0x0D
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 13
#define ESC 27
#define MUSIC_NUMBER 35
#define score_file "score.txt"
int score[MUSIC_NUMBER + 1];
int start = 0;

int restart = 0;  
int exit_game = 0;
// ���� ��Ʈ ���� (ms)
#define READ_NOTE_MIL 580
typedef struct score_information {//� �̸��� ���� ����
	char name[100];
	long long int bestscore;
}info;

void gotoxy(int x, int y) // Ŀ�� �̵� �Լ�
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
// �ܼ� ������ â�� �ڵ鰪
static HWND hWnd;
// ���α׷��� �ν��Ͻ� �ڵ鰪
static HINSTANCE hInst;

// ����� ��Ʈ
HFONT font;
HFONT font_combo;
HFONT font_score;
HFONT font_acc;
RECT rt;

// ��Ʈ ����
int note_width = 0;
int note_height = 0;

// Ÿ�̸� ����
int PlayTimer = 0;
int map_playing = FALSE;
int beg_time = 0;

// ���� �̹��� ��ġ ����
int Start_Pos = 150;

// �Ͻ����� üũ ����
int IsPause = FALSE;

// Ű �Է� üũ ����
int KeyDown[4] = { 0 };
int KeyLight[4] = { 0 };

char* NoteMapName = "";

int condition = 0;
// ������ �ڵ� ����
HANDLE TimerHandle;
HANDLE KeyPressHandle;
HANDLE PanTimeHandle;
HANDLE GameHandle;

void ClearCursor();						// Ŀ�� �ȶ߰�
void* music_timer(void* a);					// ���� Ÿ�̸� (������ �Լ�)
void checkkey(int k);				// Ű �Է� ó��. k : Ű ��ġ (0,1,2,3)
void checknote();							// ��Ʈ Ŭ�� üũ
void* keycheck(void* a);			// Ű �������� üũ (������ �Լ�)
void* acc_Timer(void* a);				// ���� ǥ�� �ð� Ÿ�̸�
void GameOver();						// ���� ���� �Լ� (���� ������ ����)
void* GameStatusCheck(void* a);			// ���� ���� üũ (������ �Լ�)
void RT_Change(RECT* rts, int a, int b, int c, int d); // ���� ��ġ ����
inline void Rendering();					// ���� �÷��̽� ������ �Լ�
void CheckPlayStatus();					//�÷��� ���� üũ
void LoadMap(char* MapName);			// �� �ҷ�����

void PlayMap();							// ���� �÷���		
void Restart();							// ����� �Լ�
void GameOverView();					// ���� ���� ��
void* ScoreIncVisual(int* Score_VS);	// ���ھ� �ε巴�� �ö󰡰� �ϱ� (������ �Լ�)
void Result();							// ��� ���

int mode = 0;

void load_score() { //score.txt�� ����Ǿ� �ִ� ������ �ҷ��ͼ� ����ü�� �迭�� ����
	info s[MUSIC_NUMBER+2];
	FILE* fp = fopen(score_file,"r");
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
	fclose(fp); 
	for (int i = 0; i <= MUSIC_NUMBER; i++) {
		score[i] = s[i].bestscore;
	}
	return;
}
void save_score() { //���� ���� �Լ�
	FILE* fp = fopen(score_file,"w");
	for(int i=0;i<MUSIC_NUMBER;i++)
		fprintf(fp,"Music%d %d\n",i,score[i]);
	fclose(fp);
}

/**
�� ���̵�
0 : ������
1 : �Ķ���
2 : �ʷϻ�
3 : ����
4 : ������
5 : ���ֻ�
6 : �����
7 : ���
8 : ȸ��
9 : ���� �Ķ���
10 : ���� �ʷϻ�
11 : ���� ����
12 : ���� ������
13 : ���� ���ֻ�
14 : ���� �����
15 : ���� ȸ��


������
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
**/
int lines[36][10][10] = { //�뷡 ��ȣ�� icon�迭
						  {{8,8,8,8,8,8,8,8,8,8},
						   {1,1,8,8,8,8,8,8,1,8},
						  {1,1,1,8,8,8,8,1,1,8},
						  {8,8,8,8,8,8,1,1,1,1},
						  {8,8,0,0,0,15,8,8,8,8},
					      {8,0,15,0,15,8,0,0,0,0},
						  {0,15,0,15,8,0,0,0,0,15},
						  {15,8,8,8,0,0,0,0,15,8},
						  {8,0,0,0,0,0,0,15,8,0},
						  {0,0,0,0,0,15,15,15,0,0} },

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

						  {{0,0,0,0,0,0,0,0,0,0},
						  {4,4,4,7,7,7,7,1,1,1},
						  {0,0,4,7,0,0,0,1,0,1},
						  {0,0,4,7,0,0,0,1,0,1},
						  {4,4,4,7,7,7,7,0,0,1},
						  {0,0,4,0,0,0,7,0,0,1},
						  {0,0,4,0,0,0,7,0,0,1},
						  {4,4,4,7,7,7,7,0,0,1},
						  {0,0,0,0,0,0,0,0,0,0},
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

						{{5,5,5,5,5,5,5,5,5,5},
						 {5,5,5,5,5,5,5,5,5,5},
						 {5,5,5,5,5,5,6,6,6,5},
						 {5,5,5,5,5,5,6,6,6,5},
						 {5,5,5,5,5,5,6,6,6,5},
						 {5,5,5,5,5,4,4,4,4,4},
						 {5,5,5,4,4,4,13,13,13,13},
						{13,13,13,13,13,13,13,13,13,13},
						 {13,13,13,13,13,13,13,13,13,13},
						 {13,13,13,13,13,13,13,13,13,13} },

						{{0,0,0,0,0,0,0,0,0,0},
						 {0,0,0,7,7,7,7,0,0,0},
						 {0,0,7,0,0,0,0,7,0,0},
						 {0,7,0,0,0,0,7,8,7,0},
						 {0,7,0,0,0,7,8,0,7,0},
						 {0,7,0,0,0,8,0,0,7,0},
						 {0,7,0,0,0,0,0,0,7,0},
						 {0,8,7,0,0,0,0,7,8,0},
						 {0,0,8,7,7,7,7,8,0,0},
						 {0,0,0,8,8,8,8,0,0,0} },

						 {{9,9,9,9,9,9,9,9,9,9},
						 {9,9,9,9,9,9,12,12,9,9},
						 {9,9,9,9,9,9,12,12,9,9},
						 {9,9,9,9,9,9,9,9,9,9},
						 {9,10,10,10,9,9,9,9,10,10},
						 {10,10,10,10,10,9,9,10,10,10},
						 {10,10,10,10,11,11,11,11,11,11},
						 {11,11,11,11,11,11,11,11,11,11},
						 {11,11,11,11,3,3,3,3,3,3},
						 {11,3,3,3,3,3,3,3,3,3} },
						  
						 {{9,9,1,1,9,9,9,9,1,9},
						  {9,1,1,1,1,9,9,9,1,1},
						  {9,9,9,9,9,9,9,9,9,9},
						  {9,9,9,9,9,9,9,9,9,9},
						  {9,9,9,9,9,11,11,11,11,11},
						  {9,9,11,11,11,11,11,11,11,11},
						  {11,11,11,11,11,11,11,11,11,11},
						  {11,11,11,11,11,11,11,11,3,3},
						  {11,11,11,11,11,3,3,3,3,3},
						  {11,11,11,3,3,3,3,3,3,3} },

						{{10,10,10,10,14,14,14,1,1,14},
						 {10,10,10,10,14,14,1,1,14,14},
						 {10,4,10,10,14,14,1,14,14,14},
						 {10,4,4,10,14,14,14,14,14,14},
						 {10,10,4,4,14,14,14,14,14,14},
						 {9,9,9,4,4,12,12,2,2,12},
						 {9,9,9,9,9,12,2,2,12,12},
						 {9,6,6,9,9,2,2,12,12,12},
						 {6,6,9,9,2,2,12,12,12,12},
						 {6,9,9,9,2,12,12,12,12,12} },

						 {{13,5,5,13,13,13,13,13,13,5},
						 {5,5,5,5,13,13,13,13,5,5},
						 {13,13,10,10,10,13,13,13,13,13},
						 {13,10,2,2,2,10,13,13,13,13},
						 {14,2,14,0,14,2,14,14,14,14},
						 {14,14,14,0,14,14,14,14,14,14},
						 {14,14,14,14,14,14,14,14,14,14},
						 {14,14,14,14,14,14,11,11,11,11},
						 {14,14,11,11,11,11,11,11,3,3},
						 {11,11,11,11,3,3,3,3,3,3} },

						 {{4,4,4,4,6,6,6,6,6,6},
						  {4,4,6,6,6,6,14,14,14,6},
						  {6,6,6,6,6,6,14,14,14,6},
						  {6,6,6,6,6,4,14,14,14,4},
						  {6,6,6,6,4,4,4,4,4,4},
						  {6,6,6,6,6,6,6,6,6,6},
						  {9,9,9,9,9,9,9,1,1,1},
						  {1,1,1,1,1,1,9,9,9,1},
						  {1,1,1,1,1,1,1,1,1,1},
						  {1,1,1,1,1,1,1,1,1,1} },

						  { {8,8,8,8,8,8,8,8,8,8},
							 {8,0,0,8,8,8,8,8,8,8},
							 {8,0,0,0,0,8,8,0,0,8},
							 {0,0,0,0,8,8,0,0,0,0},
							 {8,1,8,8,8,0,0,0,0,8},
							 {1,8,0,0,8,8,8,8,1,8},
							 {1,0,0,0,0,0,8,1,8,8},
							 {8,1,0,0,0,0,0,8,1,8},
							 {1,8,8,8,1,8,8,8,8,1},
							 {8,8,8,1,8,8,8,8,1,8} },

						  { {7,7,7,7,7,7,7,7,7,7},
						   {7,0,0,0,0,0,0,0,0,7},
						   {7,0,2,2,0,2,2,2,0,7},
						   {7,0,0,0,2,2,2,0,0,7},
						   {7,0,0,0,0,0,0,0,0,7},
						   {7,7,7,7,7,7,7,7,7,7},
						   {0,0,0,0,7,8,0,0,0,0},
						   {0,0,0,7,7,7,8,0,0,0},
						   {0,8,8,8,8,7,7,8,8,0},
						   {0,0,0,0,0,0,0,0,0,0} },

						   { {14,14,14,14,14,14,14,14,14,14},
							 {14,14,14,14,14,14,15,15,11,14},
							 {14,14,14,14,14,15,15,13,11,14},
							 {14,14,14,14,15,15,13,13,11,14},
							 {14,14,14,15,15,13,13,13,11,14},
							 {14,14,15,15,13,13,13,5,11,14},
							 {14,15,15,13,13,13,5,13,11,14},
							 {14,15,13,13,13,5,13,3,3,14},
							 {14,11,11,11,11,11,11,3,11,3},
							 {14,14,14,14,14,14,14,14,3,11} },

						{ {10,10,10,10,10,10,10,10,10,10},
						 {10,2,2,2,2,10,10,10,10,2},
						 {2,7,8,7,2,2,2,2,2,2},
						 {2,7,7,7,2,2,7,8,7,2},
						 {11,7,8,7,0,8,7,7,7,11},
						 {11,7,7,7,8,8,7,8,7,11},
						 {11,7,8,7,0,8,7,7,7,11},
						 {11,7,7,7,8,8,7,8,7,11},
						 {3,3,3,8,0,8,7,7,7,3},
						 {3,3,3,8,8,8,3,3,3,3} },

						 { {0,0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0,0},
							{0,0,0,2,14,12,2,0,0,0},
							{0,0,2,2,2,2,14,2,0,0},
							{0,2,14,2,2,14,12,14,2,0},
							{2,2,2,12,2,2,12,2,2,2},
							{12,2,12,2,14,2,2,2,14,2},
							{14,2,2,2,2,2,2,12,2,12},
							{2,2,2,14,2,12,14,2,14,0},
							{0,0,14,12,2,2,2,12,0,0} },

						 { {2,2,2,2,2,2,2,2,2,2},
							 {2,15,15,15,0,0,0,15,15,2},
							 {2,15,15,15,0,0,0,15,15,2},
							 {2,15,15,15,0,0,0,15,15,2},
							 {2,15,15,0,15,0,15,15,15,2},
							 {2,15,15,15,0,0,0,0,15,2},
							 {2,15,0,15,15,0,15,15,15,2},
							 {2,15,0,0,0,0,0,0,15,2},
							 {2,15,15,15,15,15,15,0,15,2},
							 {2,2,2,2,2,2,2,2,2,2} },

						  { {7,7,7,7,7,7,7,7,7,7},
							 {9,9,9,15,15,15,15,9,9,9},
							 {9,9,9,9,9,9,9,9,9,9},
							 {9,9,9,9,9,9,9,9,9,9},
							 {9,9,0,0,0,0,0,0,9,9},
							 {9,9,8,8,8,8,8,8,9,9},
							 {7,7,7,7,7,0,7,7,7,7},
							 {14,14,14,14,14,0,14,14,14,14},
							 {7,7,7,7,7,7,7,7,7,7},
							 {7,7,7,7,7,7,7,7,7,7} },

						 { {7,7,7,7,7,7,7,7,7,7},
						  {7,7,7,7,7,4,7,7,7,7},
						  {7,7,7,7,4,12,4,7,7,7},
						  {7,7,7,4,12,12,4,7,7,7},
						  {7,7,4,12,6,6,12,4,7,7},
						  {7,7,4,6,14,14,12,4,7,7},
						  {7,7,7,4,6,6,4,7,7,7},
						  {7,7,7,7,4,4,7,7,7,7},
						  {7,7,7,7,5,5,7,7,7,7},
						  {7,7,7,7,5,5,7,7,7,7} },

						 { {14,0,14,0,14,14,6,0,0,14},
							 {0,14,0,6,0,6,0,14,0,6},
							 {0,14,0,6,6,0,14,14,14,0},
							 {14,0,6,0,0,6,0,14,0,14},
							 {6,0,0,14,0,0,6,0,14,14},
							 {6,0,14,0,6,14,14,0,6,0},
							 {0,6,0,6,14,0,0,6,0,6},
							 {14,0,6,0,0,6,14,14,6,0},
							 {0,14,0,6,6,0,0,6,0,14},
							 {14,14,14,0,6,0,14,0,14,14} },

						  { {0,0,0,0,8,8,8,8,7,7},
							 {0,0,0,8,8,8,8,7,7,7},
							 {0,0,8,8,8,8,7,7,7,15},
							 {0,8,8,8,8,7,7,7,15,15},
							 {8,8,8,8,7,7,7,15,15,15},
							 {8,8,8,7,7,7,15,15,15,15},
							 {8,8,7,7,7,15,15,15,15,15},
							 {8,7,7,7,15,15,15,15,15,15},
							 {7,7,7,15,15,15,15,15,15,15},
							 {7,7,15,15,15,15,15,15,15,15} },

							{ {0,4,15,15,15,15,0,0,0,0},
							{15,15,4,15,15,15,15,0,0,0},
							{15,15,15,4,4,15,15,15,0,0},
							{15,15,15,4,4,4,15,15,15,0},
							{15,15,15,4,4,4,15,15,15,15},
							{15,15,15,15,15,15,4,15,15,0},
							{0,15,15,15,15,15,15,4,0,0},
							{0,0,15,15,15,15,15,0,0,0},
							{0,0,0,15,15,15,0,0,0,0},
							{0,0,0,0,15,0,0,0,0,0} },

						   { {8, 8, 8, 12, 12, 12, 12, 4, 8, 8},
							   { 8,8,8,8,8,8,8,12,4,8 },
							   { 8,8,4,4,4,4,8,8,12,4 },
							   { 8,4,12,12,12,12,4,8,8,4 },
							   { 4,12,8,8,8,8,4,12,8,4 },
							   { 4,8,8,4,8,8,4,12,8,4 },
							   { 4,8,8,12,4,4,12,8,8,4 },
							   { 12,4,8,8,12,12,8,8,4,12 },
							   { 8,12,4,8,8,8,8,4,12,8 },
							   { 8,8,12,4,4,4,4,12,8,8 } },

						{ {11,11,11,11,11,4,4,11,11,11},
						{11,11,11,4,4,4,12,4,11,11},
						{11,11,4,12,12,12,12,4,11,11},
						{11,4,12,12,14,14,12,4,11,11},
						{11,4,12,14,14,14,12,4,11,11},
						{11,4,12,14,14,14,12,4,4,11},
						{0,4,12,6,6,6,12,12,4,0},
						{15,0,0,0,0,0,0,0,0,15},
						{15,15,15,15,15,15,15,15,15,15},
						{11,15,15,15,15,15,15,15,15,11} },

							   { {5,3,3,3,3,3,3,3,3,3},
								{5,5,5,5,3,3,3,3,3,3},
								{5,5,5,5,5,5,5,5,3,3},
								{13,13,13,13,13,13,13,5,5,5},
								{13,13,13,13,13,13,13,13,13,13},
								{12,12,0,13,13,13,13,13,13,13},
								{12,12,0,12,12,12,12,12,12,12},
								{1,1,0,1,1,1,1,1,1,1},
								{1,0,0,1,0,0,0,1,0,1},
								{1,0,1,1,0,0,0,1,0,0} },

						 { {3,3,8,8,1,1,8,3,8,3},
							{3,8,3,1,8,8,1,8,3,8},
							{3,8,3,1,1,1,1,8,3,8},
							{3,3,8,1,8,8,1,8,3,8},
							{8,8,8,8,8,8,8,8,8,8},
							{8,8,8,8,8,8,8,8,8,8},
							{13,8,13,8,5,5,8,8,13,13},
							{8,13,8,5,8,8,5,13,8,13},
							{8,13,8,5,5,5,5,13,8,13},
							{8,13,8,5,8,8,5,8,13,13} },

						  { {13,13,13,13,13,13,13,13,13,5},
								{1,13,13,13,13,13,13,13,5,5},
								{1,1,13,13,13,13,13,5,5,5},
								{1,1,1,13,13,13,5,5,5,5},
								{1,1,1,1,13,5,5,5,5,5},
								{1,1,1,1,1,5,5,5,5,5},
								{1,1,1,1,0,0,5,5,5,5},
								{1,1,1,0,0,0,0,5,5,5},
								{1,1,0,0,0,0,0,0,5,5},
								{1,0,0,0,0,0,0,0,0,5} },

						 { {4,12,12,12,0,0,0,0,0,0},
							{12,0,0,12,0,0,0,0,0,0},
							{12,0,4,12,0,0,0,0,0,0},
							{12,12,12,12,4,0,0,0,0,0},
							{0,0,0,0,4,0,0,0,0,0},
							{0,0,0,0,0,4,12,12,0,0},
							{0,0,0,0,0,12,0,0,0,0},
							{0,0,0,0,0,12,12,4,0,0},
							{0,0,0,0,0,12,0,0,0,0},
							{0,0,0,0,0,12,12,12,0,4} },

						 { {15,15,15,15,15,15,15,15,15,15},
							{15,0,0,15,15,15,15,0,0,15},
							{15,0,8,8,8,8,8,8,0,15},
							{15,15,8,0,8,8,0,8,15,15},
							{15,8,8,8,8,8,8,8,8,15},
							{15,7,8,8,7,7,8,8,7,15},
							{15,7,7,7,12,12,7,7,7,15},
							{15,15,7,7,7,7,7,7,15,15},
							{15,15,15,15,15,15,15,15,15,15},
							{15,15,15,15,15,15,15,15,15,15} },

						  { {0,0,3,0,3,0,0,0,0,0},
							{0,3,14,3,14,4,0,4,0,0},
							{0,3,14,14,4,12,4,12,4,0},
							{0,3,14,14,4,12,12,12,4,0},
							{0,3,14,14,4,12,12,12,4,0},
							{0,0,3,3,4,12,12,12,4,0},
							{0,0,0,2,0,4,4,4,0,0},
							{0,2,0,2,0,2,10,0,10,10},
							{0,2,2,2,2,0,10,10,10,0},
							{0,0,2,2,0,0,10,0,0,0} },

							{ {0,0,0,0,4,4,0,0,0,0},
								{0,0,0,4,4,4,4,0,0,0},
								{0,0,0,4,12,12,4,0,0,0},
								{0,4,0,4,14,14,4,0,4,0},
								{0,12,0,0,8,8,0,0,12,0},
								{0,8,0,0,8,8,0,0,8,0},
								{0,8,0,0,8,8,0,0,8,0},
								{0,8,0,0,8,8,0,0,8,0},
								{0,8,0,0,8,8,0,0,8,0},
								{0,8,0,0,8,8,0,0,8,0} },

						{ {0,0,0,3,3,9,1,0,0,0},
							{0,0,3,9,1,3,9,1,0,0},
							{0,3,9,1,0,0,3,9,1,0},
							{0,3,9,1,0,0,3,9,1,0},
							{0,0,0,0,0,0,3,9,1,0},
							{0,0,0,3,3,3,9,1,0,0},
							{0,0,0,3,9,1,0,0,0,0},
							{0,0,0,3,9,1,0,0,0,0},
							{0,0,0,0,0,0,0,0,0,0},
							{0,0,0,3,9,1,0,0,0,0} },

							   { {11,4,4,11,11,11,11,11,11,11},
								{11,4,4,11,11,11,11,11,11,11},
								{11,11,11,11,11,2,2,2,2,2},
								{11,11,11,2,2,2,10,10,10,9},
								{11,2,2,10,10,10,10,9,9,9},
								{2,2,10,10,10,9,9,9,9,9},
								{2,10,10,9,9,9,9,9,9,10},
								{2,10,9,9,9,10,10,10,10,2},
								{10,9,9,9,10,10,10,10,2,2},
								{9,9,9,10,10,2,2,2,2,2} },

						 { {15,15,15,1,15,15,15,15,15,15},
							{15,15,15,1,4,15,15,15,15,15},
							{15,15,1,15,15,4,15,15,15,15},
							{15,15,1,15,15,4,15,15,15,15},
							{15,1,15,15,15,4,15,15,15,1},
							{15,1,15,15,15,15,4,15,1,15},
							{15,1,15,15,15,15,4,15,1,15},
							{15,1,15,15,15,15,4,15,1,15},
							{1,15,15,15,15,15,15,4,15,15},
							{1,15,15,15,15,15,15,15,15,15} },

						  { {0,0,0,0,0,0,0,0,0,4},
							{0,0,4,4,4,4,4,0,0,4},
							{0,4,0,4,4,4,0,0,4,4},
							{0,4,4,0,0,0,0,4,4,4},
							{0,4,4,4,0,0,4,4,4,4},
							{0,4,4,0,0,0,0,4,4,4},
							{0,4,0,0,4,4,0,0,4,4},
							{0,0,0,4,4,4,4,0,0,4},
							{0,0,4,4,4,4,4,4,0,0},
							{4,4,4,4,4,4,4,4,4,0} }

};
/**
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
**/
int setColor(int i, int j) { // �ؽ�Ʈ �� ���� �Լ�
	int Color = lines[music][i][j];
	return Color;
}

void draw_icon() // �迭�� ���� ���� icon�� �׸��� �Լ�
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
				printf("��");
			}
			else
				printf("  ");
		}
		printf("\n");
		position += 1;
	}
	Sleep(50);
}
/**
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
**/
void print_menu() { //���� ���� �޴��� �׸��� �Լ�
	ClearCursor();
	gotoxy(50, 3);
	printf("����������������������������������������������");
	gotoxy(50, 4);
	printf("��                     ��");
	gotoxy(50, 5);
	printf("��                     ��");
	gotoxy(50, 6);
	printf("��                     ��");
	gotoxy(50, 7);
	printf("��                     ��");
	gotoxy(50, 8);
	printf("��                     ��");
	gotoxy(50, 9);
	printf("��                     ��");
	gotoxy(50, 10);
	printf("��                     ��");
	gotoxy(50, 11);
	printf("��                     ��");
	gotoxy(50, 12);
	printf("��                     ��");
	gotoxy(50, 13);
	printf("��                     ��");
	gotoxy(50, 14);
	printf("����������������������������������������������");
	gotoxy(50, 19);
	printf("����������������������������������������������");
	gotoxy(56, 18);
	printf("�� Music ��");
	gotoxy(50, 20);
	printf("��                     ��");
	gotoxy(50, 21);
	printf("����������������������������������������������");
	gotoxy(54, 23);
	printf("�� Best Score ��");
	gotoxy(50, 24);
	printf("����������������������������������������������");
	gotoxy(50, 25);
	printf("��                     ��");
	gotoxy(50, 26);
	printf("����������������������������������������������");

	gotoxy(38, 28);
	printf("��~�� �ܢ�~�� ��~�� �ܢ�~�� ��~�� �ܢ�~�� ��~�� ��");

}
void GoodBye() { //������ �����ϴ� �Լ�
	system("cls");
	gotoxy(50, 15);
	printf("See You Next Time!!\n\n\n\n\n\n\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	PlaySound(TEXT("close.wav"), NULL, SND_ASYNC || SND_LOOP);
	Sleep(3000);
	exit(0);
}

void Draw(int x, int y) //�̹��� �׸��� �Լ�(����ȭ ������ ����..)
{
	// DC�� �ڵ鰪�� ���� ������ �����Ѵ�.(hDC : ����ȭ��DC, hMemDC : �̹����� ���� DC)
	// Device Context�� �׷��ȿ� �ʿ��� ��� �ɼ��� �־�� ����ü��� �� �� �ִ�.
	// �׸��׸��� �׸��� �׸� ȭ���̶� ����ȴ�.
	HDC hDC, hMemDC;
	// �ĸ�����̴�. static ������ ���������μ�, �޸��� Data������ ����Ǵ� �Լ��� ������ ������� �ʰ� �޸𸮿� �����ִ� �����̴�.
	static HDC hBackDC;
	// �̹��� ��Ʈ���� �ڵ鰪�� ������ �����̴�.
	HBITMAP hBitmap, hOldBitmap, hBackBitmap;
	// �ڵ鰪���κ��� ���� ���� ��Ʈ�� ����ü.
	BITMAP Bitmap;
	// ���� �������� Rect��(ũ��)�� ���´�. Rect�� ������, ������ �Ʒ��� ���� ������ ���簢���� ��Ÿ���� ����ü�̴�.
	RECT WindowRect;
	GetWindowRect(hWnd, &WindowRect);

	// ���� �������� DC �ڵ鰪�� ���´�. GetWindowDC(hWnd)�� �����ϴ�.
	hDC = GetDC(hWnd);
	// hDC�� ȣȯ�Ǵ� DC�� �޸𸮿� ����� �ڵ鰪�� ��ȯ�Ѵ�.
	hBackDC = CreateCompatibleDC(hDC);
	hMemDC = CreateCompatibleDC(hDC);

	// ��Ʈ�� �޸𸮸� �Ҵ��ϰ� �ڵ��� ��ȯ�Ѵ�.
	hBackBitmap = CreateCompatibleBitmap(hDC, WindowRect.right, WindowRect.bottom);
	// �׸� ��ȭ���� �غ��Ѵ�.
	hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);
	// ��Ʈ���� �ε��Ͽ� �ڵ��� ��ȯ�Ѵ�. resource.h�� ������ define�Ǿ��ִ� �ҷ��� ���ҽ��� �ε��Ѵ�.
	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	// ũ�⸦ �ޱ����� ��Ʈ�� ����ü�� ��Ʈ�� �ڵ�κ��� �����Ѵ�.
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	// �о�� ��Ʈ�� ���ҽ��� �޸� DC�� �����Ѵ�.
	SelectObject(hMemDC, hBitmap);

	// hMemDC�� �̹����� hBackDC�� ���ϴ� ��ġ�� ��Ӻ����Ų��.(����Ϸ��� �̹����� �ĸ���ۿ� �����Ų��.)
	BitBlt(hBackDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
	// hBackDC(�ĸ� ����)�� �ϼ��� �׸��� ȭ������ ��Ӻ����Ų��.
	BitBlt(hDC, x, y, x + Bitmap.bmWidth, y + Bitmap.bmHeight, hBackDC, 0, 0, SRCCOPY);

	// �޸𸮿� ������Ʈ�� �����Ѵ�.
	DeleteObject(SelectObject(hBackDC, hBackBitmap));
	DeleteObject(hBitmap);
	DeleteDC(hBackDC);
	DeleteDC(hMemDC);

	ReleaseDC(hWnd, hDC);
}

void print_title() { //���� ������ �׸��� �Լ�
	//system("cls");
	char key;
	ClearCursor();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	char maker[100] = { 'M','a','d','e',' ','b','y',' ','H','a','e','u','n',' ','a','n','d',' ','H','a','n',' ','G','y','e','o','l' };
	int i = 0;

	gotoxy(47, 25);
	while (maker) {
		printf("%c", maker[i]);
		i++;
		if (maker[i] == NULL)
			break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	gotoxy(40, 5);
	printf("|����������������������������������������|\n");
	gotoxy(40, 6);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("          Be Sound Rhythmical !!!\n");
	gotoxy(40, 7);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("|�ߣߣߣߣߣߣߣߣߣߣߣߣߣߣߣߣߣߣߣ�|\n");

	gotoxy(55, 12);
	printf("Story Mode");
	gotoxy(53, 14);
	printf("Challenge Mode");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

	gotoxy(50, 12);
	printf("��");
	mode = 0;
	while (1) {
		if (_kbhit()) {
			key = _getch();
			switch (key) {
			case UP:
				gotoxy(50, 14);
				printf("  ");
				gotoxy(50, 12);
				printf("��");
				PlaySound(TEXT("MP_Shoot Arrow.wav"), NULL, SND_ASYNC || SND_LOOP);
				mode = 0;
				break;
			case DOWN:
				gotoxy(50, 12);
				printf("  ");
				gotoxy(50, 14);
				printf("��");
				PlaySound(TEXT("MP_Shoot Arrow.wav"), NULL, SND_ASYNC || SND_LOOP);
				mode = 1;
				break;
			case ENTER:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				return;
			case ESC:
				GoodBye();

			}
		}
	}
}

void loading() { //loading ȭ�� ����Լ�
	system("cls");
	if (mode == 1) {
		PlaySound(TEXT("Applause.wav"), NULL, SND_ASYNC || SND_LOOP);
		int load = 10;
		gotoxy(44, 15);
		for (int i = 0; i < 9; i++)
			printf("��  ");
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
			printf("��");
			i++;

			Sleep(500);
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		PlaySound(NULL, 0, 0);
		start = 1;
	}
	else {
		start = 1;
	}
}
int state = 0;
char saying[100][10000] = { 
	{'L','o','n','g',' ','t','i','m','e',' ','a','g','o',' ','t','h','e','r','e',' ','w','a','s',' ','c','i','t','y'},
	{'I','t',' ','w','a','s',' ','r','i','c','h',' ','b','u','t',' ','u','n','f','o','r','t','u','n','a','t','e',' ','p','l','a','c','e',' ','f','o','r',' ','e','v','e','r','y','o','n','e'},
	{'B','u','t',' ','n','o',' ','o','n','e',' ','k','n','e','w',' ','w','h','y',' ','t','h','e',' ','c','i','t','y',' ','w','a','s',' ','l','i','k','e',' ','t','h','a','t'},
	{'S','o','l',' ','a',' ','h','i','g','h',' ','s','c','h','o','o','l',' ','s','t','u','d','e','n','t',' ','d','e','c','i','d','e','d',' ','t','o',' ','s','o','l','v','e',' ','t','h','i','s',' ','p','r','o','b','l','e','m'},
	{'B','u','t',' ','h','e',' ','d','i','d','n',' ','t',' ','k','n','e','w',' ','w','h','y',',',' ','S','o',' ','h','e',' ','w','a','s',' ','a','t',' ','l','o','s','s'},
	{'T','h','e','n',' ','t','h','e',' ','o','n','e',' ','d','a','y',' ','S','o','l',' ','f','o','u','n','d',' ','s','o','m','e','t','h','i','n','g',' ','s','t','r','a','n','g','e',' ','i','n',' ','t','h','e',' ','j','u','n','k',' ','s','h','o','p'},
	{'W','h','a','t',' ','i','s',' ','t','h','i','s',' ','t','h','i','n','g','?'},
	{'S','o','l',' ','s','e','t',' ','o','n',' ','t','h','e',' ',' c','h','a','i','r',' ','a','n','d',' ','p','u','t',' ','h','i','s',' ','h','a','n','d',' ','o','n',' ','t','h','e',' ','k','e','y','b','o','a','r','d'},

};


Image images[4] = {
	{"city.bmp", 190, 0, 1},
	{"Deemo.bmp", 0, 0, 4},
	{"piano.bmp",700,100,1},
	{"tile.bmp",0,100,2},
};

void story() {
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);
	if (state == 0) {
		images[0].isHide = FALSE;
		images[1].isHide = TRUE;
		imageLayer.imageCount = 2; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
		imageLayer.images = images;
		gotoxy(0, 50);
		imageLayer.renderAll(&imageLayer);
		gotoxy(45, 48);
		for (int i = 0; saying[state][i] != NULL; i++) {
			printf("%c", saying[state][i]);
			Sleep(40);
		}
		state++;
		Sleep(1500);
		gotoxy(0, 48);
		printf("                                                                                                                               ");
		gotoxy(40, 48);
		for (int i = 0; saying[state][i] != NULL; i++) {
			printf("%c", saying[state][i]);
			Sleep(40);
		}
		Sleep(1500);
		state++;
		gotoxy(0, 48);
		printf("                                                                                                                               ");
		gotoxy(40, 48);
		for (int i = 0; saying[state][i] != NULL; i++) {
			printf("%c", saying[state][i]);
			Sleep(40);
		}
		Sleep(1500);
		state++;
		gotoxy(0, 48);
		printf("                                                                                                                               ");
		gotoxy(34, 48);
		images[0].isHide = TRUE;
		images[1].isHide = FALSE;
		imageLayer.imageCount = 2; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
		imageLayer.images = images;
		imageLayer.renderAll(&imageLayer);
		for (int i = 0; saying[state][i] != NULL; i++) {
			printf("%c", saying[state][i]);
			Sleep(40);
		}
		Sleep(1500);
		state++;
		gotoxy(34, 48);
		printf("                                                             ");
		gotoxy(34, 48);
		images[0].isHide = TRUE;
		images[1].isHide = FALSE;
		imageLayer.imageCount = 2; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
		imageLayer.images = images;
		imageLayer.renderAll(&imageLayer);
		for (int i = 0; saying[state][i] != NULL; i++) {
			printf("%c", saying[state][i]);
			Sleep(40);
		}
		Sleep(1500);
		state++;
		gotoxy(34, 48);
		printf("                                                             ");
		gotoxy(33, 48);
		images[0].isHide = TRUE;
		images[1].isHide = TRUE;
		images[2].isHide = FALSE;
		imageLayer.imageCount = 3; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
		imageLayer.images = images;
		imageLayer.renderAll(&imageLayer);
		for (int i = 0; saying[state][i] != NULL; i++) {
			printf("%c", saying[state][i]);
			Sleep(40);
		}
		Sleep(1500);
		state++;
		gotoxy(0, 48);
		printf("                                                                                                                               ");
		gotoxy(53, 48);
		for (int i = 0; saying[state][i] != NULL; i++) {
			printf("%c", saying[state][i]);
			Sleep(40);
		}
		Sleep(1500);
		state++;
		gotoxy(0, 48);
		printf("                                                                                                                               ");
		gotoxy(33, 48);
		images[0].isHide = TRUE;
		images[1].isHide = TRUE;
		images[2].isHide = TRUE;
		images[3].isHide = FALSE;
		imageLayer.imageCount = 4; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
		imageLayer.images = images;
		imageLayer.renderAll(&imageLayer);
		for (int i = 0; saying[state][i] != NULL; i++) {
			printf("%c", saying[state][i]);
			Sleep(40);
		}
		Sleep(1500);
		return;
	}
	else {
		return;
	}
}

void ClearCursor() { // Ŀ�� �ȶ߰�
	CONSOLE_CURSOR_INFO c = { 0 };
	c.dwSize = 1;
	c.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
}

void* music_timer(void* a) // ���� Ÿ�̸� (������ �Լ�)
{
	beg_time = GetTickCount64() + 80; // ���� �ð�
	while (map_playing) {
		PlayTimer = GetTickCount64() - beg_time; // �÷��� Ÿ�� = ����ð� - ���۽ð�
		if (IsPause) {  // �Ͻ����� ���� ���
			int now_time = GetTickCount64(); // ������ �ð� ���
			while (IsPause)
			{
				continue;
			}
			now_time = GetTickCount64() - now_time;
			beg_time = beg_time + now_time; // �÷��� Ÿ�� ����� ���� ���� �ð� ����
		}
	}
	return;
}

void checkkey(int k) // Ű �Է� ó��. k : Ű ��ġ (0,1,2,3)
{
	for (int i = -140; i <= 140; i++) {
		if (PlayTimer + i < 0) continue; //�迭 ���� ������ ���� �ڵ�

		if (NotePoints[PlayTimer + i][k] == 1) //��Ʈ�� ���� ���
		{
			int abs_v = abs(i);						//������ ���ϱ� ���� ������ ���밪

			if (abs_v <= 16) IncKool();				//���� 16ms �̳� : Kool
			else if (abs_v <= 35) IncCool();		//���� 35ms �̳� : Cool
			else if (abs_v <= 85) IncGood();		//���� 85ms �̳� : Good
			else if (abs_v <= 140) IncBad();		//���� 140ms �̳� : Bad
			NotePoints[PlayTimer + i][k] = 0;		//��Ʈ ģ������ ǥ��
			ImagePoints[PlayTimer + i][k] = 0;		//Rendering �Լ����� �ش� ��Ʈ ǥ�� ����
			break;
		}
	}
}

void checknote() // ��Ʈ Ŭ�� üũ
{
	if (KeyDown[0]) {    // ���� �ش� Ű�� ������ ���
		checkkey(0);   // ���̶�� checkkey�Լ� ����
		KeyDown[0] = FALSE;     //  KeyDown ������ �������� �ǵ���
		//  �� �����°��� �����ϱ� ���ؼ�
	}
	if (KeyDown[1]) { // ���� ������
		checkkey(1);
		KeyDown[1] = FALSE;
	}
	if (KeyDown[2]) {
		checkkey(2);
		KeyDown[2] = FALSE;
	}
	if (KeyDown[3]) {
		checkkey(3);
		KeyDown[3] = FALSE;
	}

	Rate = (floor(Combo / 10) * 0.85) + 1; // �޺� ���ʽ� ����
}

void* keycheck(void* a) // Ű �������� üũ (������ �Լ�)
{
	while (map_playing)
	{
		//0x0000 : ������ ���� ���� ���� ȣ�� �������� �������� ���� ����
		//0x0001 : ������ ���� ���� �ְ� ȣ�� �������� �������� ���� ����
		//0x8000 : ������ ���� ���� ���� ȣ�� �������� �����ִ� ����
		//0x8001 : ������ ���� ���� �ְ� ȣ�� �������� �����ִ� ����

		if (GetAsyncKeyState(KEY_A) & 0x0001) //Key A�� ������ ���
			KeyDown[0] = TRUE;
		if (GetAsyncKeyState(KEY_B) & 0x0001)
			KeyDown[1] = TRUE;
		if (GetAsyncKeyState(KEY_C) & 0x0001)
			KeyDown[2] = TRUE;
		if (GetAsyncKeyState(KEY_D) & 0x0001)
			KeyDown[3] = TRUE;
		checknote();
	}
	return;
}

void play_preview_music() { //���� ���� �޴����� ���� ��ȣ�� ���� �̸���� ������ ����ϴ� �Լ�
	//info s[MUSIC_NUMBER];
	PlaySound(NULL, 0, 0);
	if (music == 0) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(53, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Duel in the Mist");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("Duel in the Mist.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 1) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(57, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Swan (SSS)");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("SSS.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 2) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(56, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("357 magnum");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("1337.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 3) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(61, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Pi");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("pi.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 4) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(55, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("pporappippam");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("pporappippam.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 5) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(57, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("End Time");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("EndTime.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 6) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(58, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Island");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("Island.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 7) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(58, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Au Puh");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("Au Puh.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 8) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(58, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Origami");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("Origami.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 9) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(53, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Wonderful Paradice");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("Wonderful.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 10) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(57, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Transistor");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("Transistor.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 11) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(57, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Ragnarok");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("ragnarok.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 12) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(53, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Any Last Word?");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("anyword.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 13) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Feel My Rhythm");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("feelmy.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 14) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Inverted World");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("inverted world.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 15) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(57, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Blink");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("blink.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 16) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Run2 Run To You!!");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("runtoyou.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 17) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(57, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Station");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("station.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 18) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(56, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Like Flames");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("likeflames.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 19) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(56, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Star Dust");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("stardust.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 20) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(56, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Tiferet");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("tiferet.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 21) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Luv Letter");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("luv letter.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 22) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Rolling Beat");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("rollingbeat.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 23) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Hestia");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("hestia.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 24) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("The Worst");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("theworst.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 25) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Day By Day");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("daybyday.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 26) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Tires on Fire");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("tiresonfire.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 27) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Q.E");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("qee.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 28) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Otter PoP");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("otterpop.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 29) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("I Wonder");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("iwonder.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 30) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Delrio");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("delrio.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 31) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(53, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("What You Want");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("whatyoureallywant.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 32) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("A");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("A.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 33) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Fluctuation");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("Fluctuation.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
	else if (music == 34) {
		gotoxy(53, 20);
		printf("                  ");
		gotoxy(54, 20);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("Black Ray");
		gotoxy(53, 25);
		printf("                  ");
		gotoxy(53, 25);
		printf("%d", score[music]);
		PlaySound(TEXT("blackray.wav"), NULL, SND_ASYNC || SND_LOOP);
	}
}

void select_music() { //���� ���� �޴����� ������ �����ϴ� ����� ����ϴ� �Լ� 
	char key;
	while (1) {
		if (_kbhit()) {
			key = _getch();
			switch (key) {
			case LEFT:
				if (music != 0)
					music--;
				PlaySound(TEXT("MP_Shoot Arrow.wav"), NULL, SND_ASYNC || SND_LOOP);
				play_preview_music();
				draw_icon();
				break;
			case RIGHT:
				if (music != MUSIC_NUMBER - 1)
					music++;
				PlaySound(TEXT("MP_Shoot Arrow.wav"), NULL, SND_ASYNC || SND_LOOP);
				play_preview_music();
				draw_icon();
				break;
			case ESC:
				restart = 1;
				break;
			case ENTER:
				start = 2;
				break;
			}
			if (restart == 1 || start == 2)
				return;
		}
	}
}

void play_game_music() { // ����ڰ� ������ �����ϰ� ������ ������ �� ������ ������ ����ϰ� ���� �����ϴ� �Լ�
	background = rand() % 2;
	PlaySound(NULL, 0, 0);
	if (music == 0) {
		PlaySound(TEXT("Duel in the Mist.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Yu-Peng Chen @HOYO-MiX - Duel in the Mist (Damaree) [Katana wa Waza o Sasae].map";
	}
	else if (music == 1) {
		PlaySound(TEXT("SSS.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "IZONE - Secret Story of the Swan (Ilham) [Easy].map";
	}
	else if (music == 2) {
		PlaySound(TEXT("1337.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "xi - .357 Magnum (Akali) [4K Normal].map";
	}
	else if (music == 3) {
		PlaySound(TEXT("pi.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Jinjin - pi (Jinjin) [4K Beginner].map";
	}
	else if (music == 4) {
		PlaySound(TEXT("pporappippam.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "SUNMI - pporappippam (LovelyN) [Purple Night.map";
	}
	else if (music == 5) {
		PlaySound(TEXT("EndTime.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Cres. - End Time (AHHHHHHHHHHHHHH) [Normal].map";
	}
	else if (music == 6) {
		PlaySound(TEXT("Island.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Aiobahn - Islands (Tigerinity) [Normal].map";
	}
	else if (music == 7) {
		PlaySound(TEXT("Au Puh.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "IU - Ah puh (fvrex) [Motion sickness].map";
	}
	else if (music == 8) {
		PlaySound(TEXT("Origami.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Totemo - Origami (DannyPX) [Folding].map";
	}
	else if (music == 9) {
		PlaySound(TEXT("Wonderful.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Hino Isuka - Wonderful Paradise (Murumoo) [Normal].map";
	}
	else if (music == 10) {
		PlaySound(TEXT("Transistor.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Alexander David Bachell, Samuel Rupert Cole Sutton - Transistor (Cut Ver.) (Niko_Plays) [Maxim's Normal].map";
	}
	else if (music == 11) {
		PlaySound(TEXT("ragnarok.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Gram vs. Camellia - Ragnarok (_Kobii) [Cataclysmic Destruction  riunosk's 4K].map";
	}
	else if (music == 12) {
		PlaySound(TEXT("anyword.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "HOYO-MiX - Any Last Words (SuzumeAyase) [Angki's Normal].map";
	}
	else if (music == 13) {
		PlaySound(TEXT("feelmy.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Red Velvet - Feel My Rhythm (fvrex) [diamant's Normal].map";
	}
	else if (music == 14) {
		PlaySound(TEXT("inverted world.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "ARForest - Inverted World (Rivals_7) [Standard].map";
	}
	else if (music == 15) {
		PlaySound(TEXT("blink.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Sta - BLRINK (gogozzzx) [Hyper].map";
	}
	else if (music == 16) {
		PlaySound(TEXT("runtoyou.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Umeboshi Chazuke - Run2 Run To You!! (dendro_) [Normal].map";
	}
	else if (music == 17) {
		PlaySound(TEXT("station.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Aiyru - Station (FAMoss) [Normal].map";
	}
	else if (music == 18) {
		PlaySound(TEXT("likeflames.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "MindaRyn - Like Flames (TV Size) (LuxSayaka) [Sies' Normal].map";
	}
	else if (music == 19) {
		PlaySound(TEXT("stardust.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "T & Sugah x NCT - Stardust (feat. Miyoki) (Maxim-Miau) [Cosmic Symphony].map";
	}
	else if (music == 20) {
		PlaySound(TEXT("tiferet.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "xi feat. Sta - Tiferet (gogozzzx) [Normal].map";
	}
	else if (music == 21) {
		PlaySound(TEXT("luv letter.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "DJ Okawari - Luv letter (ASCEND2) [Say you'd love me too].map";
	}
	else if (music == 22) {
		PlaySound(TEXT("rollingbeat.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Mitsukiyo - Rolling Beat (araragigun) [Normal].map";
	}
	else if (music == 23) {
		PlaySound(TEXT("hestia.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Street - Hestia (guden) [Intermediate].map";
	}
	else if (music == 24) {
		PlaySound(TEXT("theworst.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Polyphia - The Worst (Cokii-) [Terrible].map";
	}
	else if (music == 25) {
		PlaySound(TEXT("daybyday.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "lapix - Day by day (PSYQUI Remix) (Cut Ver.) (chxu) [Advanced].map";
	}
	else if (music == 26) {
		PlaySound(TEXT("tiresonfire.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Coda - Tires on Fire (Ulis-) [Normal].map";
	}
	else if (music == 27) {
		PlaySound(TEXT("qee.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Frums - Q.E. (Pizza69) [Insane].map";
	}
	else if (music == 28) {
		PlaySound(TEXT("otterpop.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Shawn Wasabi - Otter Pop (feat. Hollis) (MocaLoca) [Ocean].map";
	}
	else if (music == 29) {
		PlaySound(TEXT("iwonder.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "rainlord. - i wonder. (Nihilese) [advanced.].map";
	}
	else if (music == 30) {
		PlaySound(TEXT("delrio.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "MiddleIsland - Delrio (Raveille) [Hard].map";
	}
	else if (music == 31) {
		PlaySound(TEXT("whatyoureallywant.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "garlagan - What You Really Want (epic man 2) [Wanting (Insane)].map";
	}
	else if (music == 32) {
		PlaySound(TEXT("A.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Le Dos-on - A (Elekton) [ex].map";
	}
	else if (music == 33) {
		PlaySound(TEXT("Fluctuation.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Zekk - Fluctuation (Reyi) [Normal].map";
	}
	else if (music == 34) {
		PlaySound(TEXT("blackray.wav"), NULL, SND_ASYNC || SND_LOOP);
		NoteMapName = "Camellia - B L A C K - R A Y (Mipha-) [A N O T H E R].map";
	}
}

void* acc_Timer(void* a) //���� ǥ�� �ð� Ÿ�̸�
{
	while (map_playing) {
		if (PanVisualTimer > 0) // ���� Ÿ�̸Ӱ� 0�̻� (300ms �̳� Ű�� ������ ���)
		{
			PanVisualTimer--; //Ÿ�̸Ӹ� 1 ���δ�
		}
		Sleep(1); //1ms ����
		while (IsPause) continue;
	}
}

void GameOver() // ���� ���� �Լ� (���� ������ ����)
{
	map_playing = FALSE; //������ ������Ŵ
}

void* GameStatusCheck(void* a) // ���� ���� üũ (������ �Լ�)
{
	ClearCursor();
	while (map_playing) {
		//if (GetAsyncKeyState(VK_ESCAPE) & 0x0001) //[ESC] Ű�� ���� ���
			//PauseView(); // ���� �Ͻ�����
		if ((HP <= 0) && (M_NoFail == 0))  //HP�� 0�� �ɰ�� (HP�� 0���� �������)
			GameOver(); // ���ӿ��� (���� ����)

		for (int i = PlayTimer - 130; i >= PlayTimer - 230; i--) // Miss ����
		{
			if (i < 0) continue; // �迭�� ���� �Լ� (�迭 �ε����� -�� ���°� ����)
			for (int j = 0; j < 4; j++)
			{
				if (NotePoints[i][j] == 1) // ġ�� ���� ��Ʈ�� ���� ���
				{
					ImagePoints[i][j] = -1; // ��Ʈ ġ�� ���������� ǥ��
					NotePoints[i][j] = -1; // Rendering���� ǥ�õ��� �ʰ� ��ũ
					IncMiss(); //Miss ���� (Pan.c �Լ�)
				}
			}
		}

		while (IsPause) continue;
	}
	return;
}

void RT_Change(RECT* rts, int a, int b, int c, int d) // ���� ��ġ ����
{
	// Rendering �Լ����� ���� ǥ�ýÿ� ��ġ �����ϴ� �Լ�
	rts->left = a;
	rts->top = b;

	rts->right = c;
	rts->bottom = d;
}

inline void Rendering() // ���� �÷��̽� ������ �Լ�
{
	hWnd = GetConsoleWindow();		// �ڽ��� �ܼ� �����츦 ������
	hInst = GetModuleHandle(NULL);	// �ܼ� �ڵ鷯 ������
	HDC hDC, hMemDC;		//ǥ���� �޸� �Ҵ�
	static HDC hBackDC;		//ǥ���� �������� �Լ� �Ҵ� (���� ���۸�)
	HBITMAP hBackBitmap, hOldBitmap, hNewBitmap; //ǥ���� ��Ʈ��
	BITMAP Bitmap;	//��Ʈ�� ����

	hDC = GetDC(hWnd);

	hMemDC = CreateCompatibleDC(hDC);
	hBackDC = CreateCompatibleDC(hDC);

	hBackBitmap = CreateCompatibleBitmap(hDC, 1000, 500);		//������ �� �ȷ�Ʈ ũ�� ����
	hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);	//ǥ�õǴ� ��Ʈ��
	if (mode != 0) {
		if (background == 1) {
			hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_ground)); // ��� �ε�
			GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hMemDC, hNewBitmap);
			GdiTransparentBlt(hBackDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // �׸���
			DeleteObject(hNewBitmap);
		}
		else {
			hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_ground1)); // ��� �ε�
			GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hMemDC, hNewBitmap);
			GdiTransparentBlt(hBackDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // �׸���
			DeleteObject(hNewBitmap);
		}
		DeleteObject(hNewBitmap); // ��Ʈ ������Ʈ ����
	}
	
	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BACK)); // ��� �ε�
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);
	GdiTransparentBlt(hBackDC, Start_Pos, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // �׸���
	DeleteObject(hNewBitmap);

	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_NOTE)); //��Ʈ �̹��� �ε�
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);

	note_width = Bitmap.bmWidth;
	note_height = Bitmap.bmHeight;
	for (int i = PlayTimer; i < PlayTimer + READ_NOTE_MIL; i++) // ȭ�鿡 �׸� ��Ʈ ����
	{
		if (PlayTimer < 0) //Ÿ�̸Ӱ� �۵����� ������ ������
			break;
		for (int j = 0; j < 4; j++) // �� Ű���� Ȯ��
		{
			if (ImagePoints[i][j] == 1) // ��ġ�� ��Ʈ�� �������
			{
				/*if (i == TimingPoints[0].time + (int)(floor(TimingPoints[0].beatLength / 4.0 * k))) k++; // ����ȭ �ڵ忴���� ���� ����������..
				else continue;*/
				GdiTransparentBlt(hBackDC, j * Bitmap.bmWidth + Start_Pos, (i - PlayTimer - 500) * (-0.9), Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
				// �ܳ�Ʈ ǥ��
			}
			else if (ImagePoints[i][j] == 2) {
				GdiTransparentBlt(hBackDC, j * Bitmap.bmWidth + Start_Pos, (i - PlayTimer - 500) * (-0.9), Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
				// �ճ�Ʈ ǥ��
			}
		}
	}
	DeleteObject(hNewBitmap); // ��Ʈ ������Ʈ ����

	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BAR)); // ������ �ε�
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);
	GdiTransparentBlt(hBackDC, Start_Pos, 449, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // �׸���
	DeleteObject(hNewBitmap);

	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_LINE2)); // ������ �ε�
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);
	GdiTransparentBlt(hBackDC, Start_Pos, 457, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // �׸���
	DeleteObject(hNewBitmap);

	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_PIANOLEFT)); // ������ �ε�
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);
	GdiTransparentBlt(hBackDC, Start_Pos-19, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // �׸���
	DeleteObject(hNewBitmap);

	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_PIANORIGHT)); // ������ �ε�
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);
	GdiTransparentBlt(hBackDC, Start_Pos + (note_width * 4), 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // �׸���
	DeleteObject(hNewBitmap);

	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_NOTE_LINE));
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);
	for (int i = 1; i < 4; i++)
		GdiTransparentBlt(hBackDC, Start_Pos + (note_width * i), 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
	DeleteObject(hNewBitmap);
	/**
	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_LINE)); // ��Ʈ ���� ���м� �ε�
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);
	GdiTransparentBlt(hBackDC, Start_Pos, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // ���� �׸���
	GdiTransparentBlt(hBackDC, Start_Pos + (note_width * 4), 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // ������ �׸���
	DeleteObject(hNewBitmap);
	
	**/

	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_HEALTH)); // HP�� �ε�
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);
	GdiTransparentBlt(hBackDC, Start_Pos + (note_width * 4) + 5, 450 - floor((double)((__int64)Bitmap.bmHeight * (HP)) / 1.5), Bitmap.bmWidth, floor(((__int64)Bitmap.bmHeight * (HP)) / 1.5), hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
	// 5*5 ��Ʈ���� HP�� ���� �ø��� ���δ�.
	// ���� ������ �̹��� ���̸� �����ؼ� �ִ��� �ڿ������� �ٹδ�
	DeleteObject(hNewBitmap);

	if (PanVisualTimer > 0) { // ���� �̹����� ǥ���ؾ� �Ұ�� (300ms �̳��� ��Ʈ�� �������)
		if (LastPan == KOOL) { // �ֱٿ� ģ ��Ʈ�� Kool �ΰ��
			hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_KOOL)); // Kool �̹��� �ε�
			GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hMemDC, hNewBitmap);
			GdiTransparentBlt(hBackDC, 238, 350, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // �׸���
			DeleteObject(hNewBitmap);
		}
		else if (LastPan == COOL) { // ���� ����
			hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_COOL));
			GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hMemDC, hNewBitmap);
			GdiTransparentBlt(hBackDC, 238, 350, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
			DeleteObject(hNewBitmap);
		}
		else if (LastPan == GOOD) {
			hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_GOOD));
			GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hMemDC, hNewBitmap);
			GdiTransparentBlt(hBackDC, 238, 350, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
			DeleteObject(hNewBitmap);
		}
		else if (LastPan == BAD) {
			hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BAD));
			GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hMemDC, hNewBitmap);
			GdiTransparentBlt(hBackDC, 238, 350, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
			DeleteObject(hNewBitmap);
		}
		else if (LastPan == MISS) {
			hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_MISS));
			GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hMemDC, hNewBitmap);
			GdiTransparentBlt(hBackDC, 238, 350, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
			DeleteObject(hNewBitmap);
		}
	}

	char buf[100]; // ǥ���� ���� ����
	SetTextColor(hBackDC, RGB(255, 255, 255)); // �ؽ�Ʈ ���� ����
	SetBkMode(hBackDC, TRANSPARENT); // ���� ���� ����

	if (Combo > 0) { // �޺��� 0 �̻��ϰ�� (�޺��� 0�� ��� ǥ�� ���ϱ� ����)
		(HFONT)SelectObject(hBackDC, font_combo); // ��Ʈ�� font_combo�� ����

		sprintf(buf, "%d", Combo); //�޺� buffer�� ����
		RT_Change(&rt, 450, 50, 960, 160); // ���� ��ġ ����
		DrawTextA(hBackDC, buf, -1, &rt, DT_CENTER); // ���� ��ο�
	}

	(HFONT)SelectObject(hBackDC, font_score); // ���ھ� ��Ʈ�� ����

	//���� ����
	sprintf(buf, "%08d", Score); // ���� ǥ��
	RT_Change(&rt, 10, 10, 950, 160);
	DrawTextA(hBackDC, buf, -1, &rt, DT_RIGHT);

	(HFONT)SelectObject(hBackDC, font_acc);

	sprintf(buf, "%.2lf%%\n", Accurate); // ��Ȯ�� ǥ��
	RT_Change(&rt, 10, 40, 950, 160);
	DrawTextA(hBackDC, buf, -1, &rt, DT_RIGHT);

	BitBlt(hDC, 0, 0, 1000, 500, hBackDC, 0, 0, SRCCOPY); // ��׶��忡�� �׸� �̹��� �ֿܼ� �׸���

	DeleteObject(SelectObject(hBackDC, hBackBitmap)); // ����� ������Ʈ ����
	DeleteObject(hNewBitmap);
	DeleteDC(hBackDC);
	DeleteDC(hMemDC);

	ReleaseDC(hWnd, hDC); // ������ �Ҵ� ����
}

void CheckPlayStatus() //�÷��� ���� üũ
{
	if (PlayTimer >= Last_Note_pos + 5000) { // ��Ʈ�� ������ ���
		map_playing = FALSE; // ���� ������
	}
}

void LoadMap(char* MapName) // �� �ҷ�����
{
	char buf[500] = { NULL }; // ���� ����
	if (LoadMapFile(MapName) == 0)
	{
		printf("Load Failed.");
	}
	sprintf(buf, "%s - %s (%s) [%s]", M_MetaData.Artist, M_MetaData.Title, M_MetaData.Creator, M_MetaData.Version);
	// ���� ������, �� �̸�, �� ������, ���� buffer�� ����
	SetConsoleTitle(buf); //���� ���� ����
}


void PlayMap() // ���� �÷���
{
	// ��Ʈ ����
	font = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Aller");
	font_combo = CreateFont(48, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Slant");
	font_score = CreateFont(36, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Slant");
	font_acc = CreateFont(32, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Cunia");

	char buf[500] = { NULL };

	// ������ ����
	TimerHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)music_timer, 0, 0, NULL); // Ÿ�̸� ������
	KeyPressHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)keycheck, 0, 0, NULL); // Ű�Է� ������
	PanTimeHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)acc_Timer, 0, 0, NULL); // ���� ǥ�� Ÿ�̸� ������
	GameHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)GameStatusCheck, 0, 0, NULL); // ������ ���� ���� ������

	map_playing = TRUE; // ���� �÷��� ����
	while (map_playing) {
		CheckPlayStatus(); // ���� ���� üũ
		Rendering(); // ���� ȭ�� ������ ����
		if (exit_game == 1)
			break;
		while (IsPause) return;    // �Ͻ������� �Լ� �Ͻ�����
	}
}


void Reset() {//���� ����۽� ���ӿ� ���̴� ������ �ʱ�ȭ �ϴ� �Լ�
	HP = 100;
	PlayTimer = 0;
	Score = 0;
	PanVisualTimer = 0;
	LastPan = -1;
	Combo = 0;
	MaxCombo = 0;
	Rate = 1.0;
	HPUpRate = 1.0;
	HPDownRate = 1.0;
	Accurate = 100.0;

	Start_Pos = 150;
	for (int i = 0; i < 5; i++)
		TargetPass[i] = 0;
	IsPause = 0;
	//map_playing = TRUE;
}
void Restart() // ����� �Լ�
{
	// ���� �ʱ�ȭ
	HP = 100;
	PlayTimer = 0;
	Score = 0;
	PanVisualTimer = 0;
	LastPan = -1;
	Combo = 0;
	MaxCombo = 0;
	Rate = 1.0;
	HPUpRate = 1.0;
	HPDownRate = 1.0;
	Accurate = 100.0;

	Start_Pos = 150;

	for (int i = 0; i < 5; i++)
		TargetPass[i] = 0;

	ClearCursor();
	LoadMap(NoteMapName);   // �� ��ε�
	map_playing = TRUE;		// �÷��� �� �Լ� Ȱ��ȭ
	PlayMap();				// �� �÷���
	Result();				// ��� ���
}

void GameOverView() // ���� ���� ��
{
	// Rendering �Լ��� �ߺ��Ǵ� �ּ��� ���� ����
	PlaySound(NULL, 0, 0);
	hWnd = GetConsoleWindow();
	hInst = GetModuleHandle(NULL);
	HDC hDC, hMemDC;
	static HDC hBackDC;
	HBITMAP hBackBitmap, hOldBitmap, hNewBitmap;
	BITMAP Bitmap;

	hDC = GetDC(hWnd);

	hMemDC = CreateCompatibleDC(hDC);
	hBackDC = CreateCompatibleDC(hDC);

	hBackBitmap = CreateCompatibleBitmap(hDC, 1280, 720);
	hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);

	char buf[200];
	(HFONT)SelectObject(hBackDC, font_score);

	SetTextColor(hBackDC, RGB(255, 255, 255));
	SetBkMode(hBackDC, TRANSPARENT);

	sprintf(buf, "You are Dead..");
	RT_Change(&rt, 20, 30, 900, 90);
	DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

	sprintf(buf, "Press [R] to retry");
	RT_Change(&rt, 20, 90, 900, 150);
	DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

	BitBlt(hDC, 0, 0, 1280, 720, hBackDC, 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hBackDC, hBackBitmap));
	DeleteDC(hBackDC);
	DeleteDC(hMemDC);

	ReleaseDC(hWnd, hDC);

	while (1) {
		if (GetAsyncKeyState(0x52) & 0x0001) {// [R] Ű�� �������
			play_game_music();
			Restart();							// �����
			break;
		}
	}
}

void* ScoreIncVisual(int* Score_VS) // ���ھ� �ε巴�� �ö󰡰� �ϱ� (������ �Լ�)
{
	for (int i = 0; i < Score; i++) // ���ھ� ��ŭ �ݺ�
	{
		(*Score_VS)++;	// ���ھ� ++
		if (i % ((Score / 1400) + 1) == 0) Sleep(2); //���� �ʿ��� Score_VS ���
		// ��, ���� �ʰ� �Ǿ߸� ���ڰ� �ݿ��� �Ǿ� �ε巴�� �ö󰡴� ������ �ȴ�
	}
}

void Result() // ��� ���
{
	if (HP <= 0) { // HP�� 0 ����, �׾ ������ ���
		GameOverView();		// ���ӿ��� �� ���
	}
	else { // �����ؼ� ���� ���
		HFONT fnt_score = CreateFont(36, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Cunia");
		HFONT fnt_result = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Cunia");
		HFONT fnt_combo = CreateFont(36, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Cunia");

		if (score[music] < Score) score[music] = Score; // �ְ������� ���� ���
		char* buffs[200] = { NULL };
		sprintf(buffs, "{\"Kool\":%d,\"Cool\":%d,\"Good\":%d,\"Bad\":%d,\"Miss\":%d,\"MaxCombo\":%d,\"Acc\":%.2lf}"
			, TargetPass[KOOL], TargetPass[COOL], TargetPass[GOOD], TargetPass[BAD], TargetPass[MISS], MaxCombo, (double)Accurate);

		int Score_View = 0; // ���̴� ���ھ� ����
		HANDLE IncHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ScoreIncVisual, &Score_View, 0, NULL);
		// ���ھ� �ε巴�� �����ϱ� ���� ������ ����

	
		
		while (1) {
			// Rendering �Լ��� �ߺ��Ǵ� �ּ��� ���� ����
			hWnd = GetConsoleWindow();
			hInst = GetModuleHandle(NULL);
			HDC hDC, hMemDC;
			static HDC hBackDC;
			HBITMAP hBackBitmap, hOldBitmap, hNewBitmap;
			BITMAP Bitmap;

			hDC = GetDC(hWnd);

			hMemDC = CreateCompatibleDC(hDC);
			hBackDC = CreateCompatibleDC(hDC);

			hBackBitmap = CreateCompatibleBitmap(hDC, 1280, 720);
			hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);

			char buf[200];
			(HFONT)SelectObject(hBackDC, fnt_score);

			SetTextColor(hBackDC, RGB(174, 189, 242));
			SetBkMode(hBackDC, TRANSPARENT);

			hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_RANKING));
			GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hMemDC, hNewBitmap);
			GdiTransparentBlt(hBackDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // �׸��� 580 0
			DeleteObject(hNewBitmap);

			hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_PASS));
			GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hMemDC, hNewBitmap);
			GdiTransparentBlt(hBackDC, 20, 400, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // �׸��� 580 0
			DeleteObject(hNewBitmap);

			if (Score == Score_View) {
				if (Accurate >= 80.0) hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_RankS)); // S ���� 12
				else if (Accurate >= 70.0) hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_RankA)); // A ���� 15 
				else if (Accurate >= 60.0) hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_RankB)); // B ����16
				else hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_RankC)); // C ���� 28
				GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
				SelectObject(hMemDC, hNewBitmap);
				GdiTransparentBlt(hBackDC, 580, 10, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // �׸��� 580 0
				DeleteObject(hNewBitmap);
			}

			sprintf(buf, "Music: %s ", M_MetaData.Title);
			RT_Change(&rt, 20, 30, 900, 90);
			DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

			sprintf(buf, "Score: %07d", Score_View); // ���ھ� ��� (�ε巴��)
			RT_Change(&rt, 20, 70, 700, 120);
			DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

			(HFONT)SelectObject(hBackDC, fnt_result);

			SetTextColor(hBackDC, RGB(200, 255, 255));
			SetBkMode(hBackDC, TRANSPARENT);

			sprintf(buf, "Kool: %d\n", TargetPass[KOOL]); //Kool, Cool... ���
			RT_Change(&rt, 20, 150, 500, 200);
			DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);
			sprintf(buf, "Cool: %d\n", TargetPass[COOL]);
			RT_Change(&rt, 200, 150, 500, 200);
			DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);
			sprintf(buf, "Good: %d\n", TargetPass[GOOD]);
			RT_Change(&rt, 20, 190, 500, 240);
			DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);
			sprintf(buf, "Bad: %d\n", TargetPass[BAD]);
			RT_Change(&rt, 200, 190, 500, 240);
			DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);
			sprintf(buf, "Miss: %d\n", TargetPass[MISS]);
			RT_Change(&rt, 20, 230, 500, 270);
			DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

			(HFONT)SelectObject(hBackDC, fnt_combo);

			SetTextColor(hBackDC, RGB(255, 255, 255));
			SetBkMode(hBackDC, TRANSPARENT);

			sprintf(buf, "MaxCombo: %d\n", MaxCombo);	// �ִ� �޺�
			RT_Change(&rt, 20, 300, 500, 370);
			DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

			sprintf(buf, "Accuracy: %.2lf%%\n", (double)Accurate); // ��Ȯ�� ���
			RT_Change(&rt, 20, 350, 500, 400);
			DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

			BitBlt(hDC, 0, 0, 1280, 720, hBackDC, 0, 0, SRCCOPY); // �̹��� ǥ��

			DeleteObject(SelectObject(hBackDC, hBackBitmap));
			DeleteDC(hBackDC);
			DeleteDC(hMemDC);

			ReleaseDC(hWnd, hDC);
			if (GetAsyncKeyState(0x52) & 0x0001) {  // [R] Ű�� �������
				break;
			}
		}
	}
}
void start_tutorial() {
	system("cls");
	Reset();
	PlaySound(NULL, 0, 0);
	ClearCursor();
	gotoxy(45, 15);
	printf("Please     press    a, s, l, ;");
	Sleep(2000);
	system("cls");
	PlaySound(TEXT("withoutend.wav"), NULL, SND_ASYNC || SND_LOOP);
	NoteMapName = "UNDEAD CORPORATION - Without the end (Maxim-Miau) [4k  The end].map";
	start = 2;
	LoadMap(NoteMapName);
	PlayMap();
	Result();
	start = 1;
	state++;
}

/** ����ȭ �� �� ���� ���� �޴� ȭ��
void select_music() {
	int music_number = 1;
	int next_music = 0;
	int before_music = 0;
	while (1) {
		if (music_number != 1 && music_number != MAX_MUSIC) {
			next_music = music_number + 1;
			before_music = music_number - 1;
		}
		else if (music_number == 1) {
			next_music = music_number + 1;
			before_music = MAX_MUSIC;
		}
		else if (music_number == MAX_MUSIC) {
			next_music = 1;
			before_music = music_number - 1;
		}
		gotoxy(55, 14);
		printf("> music %d <", music_number);
		gotoxy(57, 10);
		printf("music %d", next_music);
		gotoxy(57, 18);
		printf("music %d", before_music);
		if (GetAsyncKeyState(VK_UP) & 0x0001) { //���� ȭ��ǥ Ű�� ������ ��
			if (music_number == MAX_MUSIC)
				music_number = 1;
			else
				music_number++;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x0001) { //�Ʒ��� ȭ��ǥ Ű�� ������ ��
			if (music_number == 1)
				music_number = MAX_MUSIC;
			else
				music_number--;
		}
		if (GetAsyncKeyState(VK_RETURN) & 0x0001) {
			break;
		}
		Sleep(10);
	}
	switch (music_number) {
	case 1:
		NoteMapName = "Jinjin - pi (Jinjin) [4K Beginner].map";
		break;
	case 2:
		NoteMapName = "Halozy - K.O.K.O.R.O (Nepuri) [EZ].map";
		break;
	case 3:
		NoteMapName = "banshi - Fading Star(banshi-Remix) (erlinadewi-) [Beginner].map";
		break;
	case 4:
		NoteMapName = "sakuzyo - Scattered Rose (Starry-) [Normal].map";
		break;
	case 5:
		NoteMapName = "sakuzyo - Scattered Rose (Starry-) [Another].map";
		break;
	case 6:
		NoteMapName = "gems - Gems Pack 13 - LN Master 5th (gemboyong) [69 ^ - Randy Mortimer, Penguin (Pinnacle Remix)].map";
		break;
	case 7:
		NoteMapName = "xi - Bad Elixir (LNP-) [Syr's Basic].map";
		break;
	}
	return;
}
**/

