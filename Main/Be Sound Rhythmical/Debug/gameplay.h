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
#include "MapReader.h"
#include "resource.h"
#include "Digitalv.h"
#include "Pan.h"
#include "menu.h"
int music = 0;
int background = 0;
#pragma comment(lib,"Winmm.lib")
#define TRUE 1
#define FALSE 0
#define OEM 2
#define MAX_MUSIC 7
// 입력 받을 키 선언
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
// 읽을 노트 길이 (ms)
#define READ_NOTE_MIL 580
typedef struct score_information {
	char name[100];
	long long int bestscore;
}info;

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
// 음악 파라메터 선언
MCI_OPEN_PARMS m_mciOpenParms;
MCI_PLAY_PARMS m_mciPlayParms;
DWORD m_dwDeviceID;
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;
int dwID;

// 사용할 폰트
HFONT font;
HFONT font_combo;
HFONT font_score;
HFONT font_acc;
RECT rt;

// 노트 길이
int note_width = 0;
int note_height = 0;

// 타이머 선언
int PlayTimer = 0;
int map_playing = FALSE;
int beg_time = 0;

// 게임 이미지 위치 설정
int Start_Pos = 150;

// 일시정지 체크 변수
int IsPause = FALSE;

// 키 입력 체크 변수
int KeyDown[4] = { 0 };
int KeyLight[4] = { 0 };

// 콘솔 윈도우 조작을 위한 변수 선언
static HWND hWnd;
static HINSTANCE hInst;

// 맵 이름
// 맵을 골라주세요!
//char* NoteMapName = "Jinjin - pi (Jinjin) [4K Beginner].map";          // 00:33 난이도 최하
//char* NoteMapName = "Halozy - K.O.K.O.R.O (Nepuri) [EZ].map";			// 01:17 난이도 하
//char* NoteMapName = "banshi - Fading Star(banshi-Remix) (erlinadewi-) [Beginner].map"; // 02:33 난이도 중
char* NoteMapName = "Jinjin - pi (Jinjin) [4K Beginner].map"; // 00:57 난이도 상
//char* NoteMapName = "sakuzyo - Scattered Rose (Starry-) [Another].map";
//char* NoteMapName = "gems - Gems Pack 13 - LN Master 5th (gemboyong) [69 ^ - Randy Mortimer, Penguin (Pinnacle Remix)].map"; // 01:35 난이도 최상, 프레임 테스트용 맵. 막 눌러주세요!
;
int condition = 0;
// 스레드 핸들 선언
HANDLE TimerHandle;
HANDLE KeyPressHandle;
HANDLE PanTimeHandle;
HANDLE GameHandle;

//// 기본 함수 ////
int UserDataLoad();						// 최고 점수 불러오기
void UserDataWrite(int score);			// 최고 점수 기록
void ClearCursor();						// 커서 안뜨게
void* M_Timer(void* a);					// 게임 타이머 (스레드 함수)
void KeyDownProcess(int k);				// 키 입력 처리. k : 키 위치 (0,1,2,3)
void HitNote();							// 노트 클릭 체크
void* CheckKeyPress(void* a);			// 키 눌렀는지 체크 (스레드 함수)
void* Pan_Timer(void* a);				// 판정 표시 시간 타이머
void GameOver();						// 게임 오버 함수 (게임 오버시 실행)
void* GameStatusCheck(void* a);			// 게임 상태 체크 (스레드 함수)
void RT_Change(RECT* rts, int a, int b, int c, int d); // 글자 위치 변경
inline void Render();					// 게임 플레이시 렌더링 함수
void CheckPlayStatus();					//플레이 상태 체크
void LoadMap(char* MapName);			// 맵 불러오기

//// 음악 함수 ////

void MusicLoad();						// 음악 불러오기
void MusicOpen(char* FileName);			// 원하는 음악 불러오기
void MusicPlay(int ID, int repeat);		// 음악 재생
void MusicPause(int ID);				// 음악 일시정지
void MusicResume(int ID);				// 음악 다시 재생
void MusicStop(int ID);					// 음악 완전히 정지
void SetMusicPosition(DWORD dwPosition);// 음악 플레이 위치 지정

//// 게임 진행 함수 ////
void PlayMap();							// 게임 플레이
void ReadyRender();						// 준비화면 렌더링
void ReadyView();						// 준비 뷰
void PauseView();						// 일시정지 뷰
void Restart();							// 재시작 함수
void GameOverView();					// 게임 오버 뷰
void* ScoreIncVisual(int* Score_VS);	// 스코어 부드럽게 올라가게 하기 (스레드 함수)
void Result();							// 결과 출력

//*/////////////////////////////////// = 선언부 종료 = ///////////////////////////////////*//
void load_score() {
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
void save_score() {
	FILE* fp = fopen(score_file,"w");
	for(int i=0;i<MUSIC_NUMBER;i++)
		fprintf(fp,"Music%d %d\n",i,score[i]);
	fclose(fp);
}

/**
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
**/
int lines[36][10][10] = {
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
	HFONT fnt_score = CreateFont(36, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Cunia");
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
	GdiTransparentBlt(hBackDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // 그리기 580 0
	DeleteObject(hNewBitmap);

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
	DeleteObject(SelectObject(hBackDC, hBackBitmap)); // 사용한 오브젝트 정리
	DeleteObject(hNewBitmap);
	DeleteDC(hBackDC);
	DeleteDC(hMemDC);

	ReleaseDC(hWnd, hDC); // 윈도우 할당 해제
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

int UserDataLoad() //최고 점수 불러오기
{
	FILE* fp;
	int index = 0;
	int data;

	fp = fopen("score.txt", "r");
	while (fscanf(fp, "%d", &data) != EOF) {
		index = (int)data;
	}
	fclose(fp);
	return index; // 최고점수 리턴
}

void UserDataWrite(int score) // 최고 점수 기록
{
	FILE* fp;

	fp = fopen("score.txt", "w");
	fprintf(fp, "%d\n", score); // 스코어 기록
	fclose(fp);
}

void ClearCursor() { // 커서 안뜨게
	CONSOLE_CURSOR_INFO c = { 0 };
	c.dwSize = 1;
	c.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
}

void* M_Timer(void* a) // 게임 타이머 (스레드 함수)
{
	beg_time = GetTickCount64() + 80; // 시작 시간
	while (map_playing) {
		PlayTimer = GetTickCount64() - beg_time; // 플레이 타임 = 현재시간 - 시작시간
		if (IsPause) {  // 일시정지 했을 경우
			int now_time = GetTickCount64(); // 중지된 시간 기록
			while (IsPause)
			{
				continue;
			}
			now_time = GetTickCount64() - now_time;
			beg_time = beg_time + now_time; // 플레이 타임 계산을 위한 시작 시간 수정
		}
	}
	return;
}

void KeyDownProcess(int k) // 키 입력 처리. k : 키 위치 (0,1,2,3)
{
	for (int i = -140; i <= 140; i++) {
		if (PlayTimer + i < 0) continue; //배열 범위 규정을 위한 코드

		if (NotePoints[PlayTimer + i][k] == 1) //노트가 있을 경우
		{
			int abs_v = abs(i);						//오차를 구하기 위해 오차의 절대값

			if (abs_v <= 16) IncKool();				//오차 16ms 이내 : Kool
			else if (abs_v <= 35) IncCool();		//오차 35ms 이내 : Cool
			else if (abs_v <= 85) IncGood();		//오차 85ms 이내 : Good
			else if (abs_v <= 140) IncBad();		//오차 140ms 이내 : Bad
			NotePoints[PlayTimer + i][k] = 0;		//노트 친것으로 표시
			ImagePoints[PlayTimer + i][k] = 0;		//Render 함수에서 해당 노트 표시 안함
			break;
		}
	}
}

void HitNote() // 노트 클릭 체크
{
	if (KeyDown[0]) {    // 현재 해당 키를 눌렀을 경우
		KeyDownProcess(0);   // 참이라면 KeyDownProcess함수 실행
		KeyDown[0] = FALSE;     //  KeyDown 변수를 거짓으로 되돌림
		//  꾹 누르는것을 방지하기 위해서
	}
	if (KeyDown[1]) { // 위와 동일함
		KeyDownProcess(1);
		KeyDown[1] = FALSE;
	}
	if (KeyDown[2]) {
		KeyDownProcess(2);
		KeyDown[2] = FALSE;
	}
	if (KeyDown[3]) {
		KeyDownProcess(3);
		KeyDown[3] = FALSE;
	}

	Rate = (floor(Combo / 10) * 0.85) + 1; // 콤보 보너스 수정
}

void* CheckKeyPress(void* a) // 키 눌렀는지 체크 (스레드 함수)
{
	while (map_playing)
	{
		//0x0000 : 이전에 누른 적이 없고 호출 시점에도 눌려있지 않은 상태
		//0x0001 : 이전에 누른 적이 있고 호출 시점에는 눌려있지 않은 상태
		//0x8000 : 이전에 누른 적이 없고 호출 시점에는 눌려있는 상태
		//0x8001 : 이전에 누른 적이 있고 호출 시점에도 눌려있는 상태

		if (GetAsyncKeyState(KEY_A) & 0x0001) //Key A를 눌렀을 경우
			KeyDown[0] = TRUE;
		if (GetAsyncKeyState(KEY_B) & 0x0001)
			KeyDown[1] = TRUE;
		if (GetAsyncKeyState(KEY_C) & 0x0001)
			KeyDown[2] = TRUE;
		if (GetAsyncKeyState(KEY_D) & 0x0001)
			KeyDown[3] = TRUE;
		HitNote();
	}
	return;
}

void play_preview_music() {
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

void select_music() {
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

void play_game_music() {
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

void* Pan_Timer(void* a) //판정 표시 시간 타이머
{
	while (map_playing) {
		if (PanVisualTimer > 0) // 판정 타이머가 0이상 (300ms 이내 키를 눌렀을 경우)
		{
			PanVisualTimer--; //타이머를 1 줄인다
		}
		Sleep(1); //1ms 쉬기
		while (IsPause) continue;
	}
}

void GameOver() // 게임 오버 함수 (게임 오버시 실행)
{
	map_playing = FALSE; //게임을 중지시킴
}

void* GameStatusCheck(void* a) // 게임 상태 체크 (스레드 함수)
{
	ClearCursor();
	while (map_playing) {
		//if (GetAsyncKeyState(VK_ESCAPE) & 0x0001) //[ESC] 키를 누를 경우
			//PauseView(); // 게임 일시정지
		if ((HP <= 0) && (M_NoFail == 0))  //HP가 0이 될경우 (HP가 0보다 작을경우)
			GameOver(); // 게임오버 (게임 중지)

		for (int i = PlayTimer - 130; i >= PlayTimer - 230; i--) // Miss 판정
		{
			if (i < 0) continue; // 배열을 위한 함수 (배열 인덱스에 -가 들어가는걸 방지)
			for (int j = 0; j < 4; j++)
			{
				if (NotePoints[i][j] == 1) // 치지 않은 노트가 있을 경우
				{
					ImagePoints[i][j] = -1; // 노트 치지 않은것으로 표시
					NotePoints[i][j] = -1; // Render에서 표시되지 않게 마크
					IncMiss(); //Miss 증가 (Pan.c 함수)
				}
			}
		}

		while (IsPause) continue;
	}
	return;
}

void RT_Change(RECT* rts, int a, int b, int c, int d) // 글자 위치 변경
{
	// Render 함수에서 글자 표시시에 위치 지정하는 함수
	rts->left = a;
	rts->top = b;

	rts->right = c;
	rts->bottom = d;
}

inline void Render() // 게임 플레이시 렌더링 함수
{
	hWnd = GetConsoleWindow();		// 자신의 콘솔 윈도우를 가져옴
	hInst = GetModuleHandle(NULL);	// 콘솔 핸들러 가져옴
	HDC hDC, hMemDC;		//표시할 메모리 할당
	static HDC hBackDC;		//표시전 렌더링할 함수 할당 (더블 버퍼링)
	HBITMAP hBackBitmap, hOldBitmap, hNewBitmap; //표시할 비트맵
	BITMAP Bitmap;	//비트맵 선언

	hDC = GetDC(hWnd);

	hMemDC = CreateCompatibleDC(hDC);
	hBackDC = CreateCompatibleDC(hDC);

	hBackBitmap = CreateCompatibleBitmap(hDC, 1000, 500);		//렌더링 할 팔레트 크기 선언
	hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);	//표시되는 비트맵

	if (background == 1) {
		hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_ground)); // 배경 로드
		GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
		SelectObject(hMemDC, hNewBitmap);
		GdiTransparentBlt(hBackDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // 그리기
		DeleteObject(hNewBitmap);
	}
	else {
		hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_ground1)); // 배경 로드
		GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
		SelectObject(hMemDC, hNewBitmap);
		GdiTransparentBlt(hBackDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // 그리기
		DeleteObject(hNewBitmap);
	}

	DeleteObject(hNewBitmap); // 노트 오브젝트 삭제
	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BACK)); // 배경 로드
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);
	GdiTransparentBlt(hBackDC, Start_Pos, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // 그리기
	DeleteObject(hNewBitmap);

	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_NOTE)); //노트 이미지 로드
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);

	note_width = Bitmap.bmWidth;
	note_height = Bitmap.bmHeight;
	for (int i = PlayTimer; i < PlayTimer + READ_NOTE_MIL; i++) // 화면에 그릴 노트 범위
	{
		if (PlayTimer < 0) //타이머가 작동하지 않으면 나가기
			break;
		for (int j = 0; j < 4; j++) // 각 키마다 확인
		{
			if (ImagePoints[i][j] == 1) // 위치에 노트가 있을경우
			{
				/*if (i == TimingPoints[0].time + (int)(floor(TimingPoints[0].beatLength / 4.0 * k))) k++; // 최적화 코드였으나 구현 귀찮음으로..
				else continue;*/
				GdiTransparentBlt(hBackDC, j * Bitmap.bmWidth + Start_Pos, (i - PlayTimer - 500) * (-0.9), Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
				// 단노트 표시
			}
			else if (ImagePoints[i][j] == 2) {
				GdiTransparentBlt(hBackDC, j * Bitmap.bmWidth + Start_Pos, (i - PlayTimer - 500) * (-0.9), Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
				// 롱노트 표시
			}
		}
	}
	DeleteObject(hNewBitmap); // 노트 오브젝트 삭제

	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_BAR)); // 판정선 로드
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);
	GdiTransparentBlt(hBackDC, Start_Pos, 450, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // 그리기
	DeleteObject(hNewBitmap);

	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_LINE2)); // 판정선 로드
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);
	GdiTransparentBlt(hBackDC, Start_Pos, 455, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // 그리기
	DeleteObject(hNewBitmap);

	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_LINE)); // 노트 양쪽 구분선 로드
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);
	GdiTransparentBlt(hBackDC, Start_Pos, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // 왼쪽 그리기
	GdiTransparentBlt(hBackDC, Start_Pos + (note_width * 4), 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // 오른쪽 그리기
	DeleteObject(hNewBitmap);
	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_NOTE_LINE));
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);
	for(int i=1;i<4;i++)
		GdiTransparentBlt(hBackDC, Start_Pos + (note_width * i), 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
	DeleteObject(hNewBitmap);

	hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_HEALTH)); // HP바 로드
	GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
	SelectObject(hMemDC, hNewBitmap);
	GdiTransparentBlt(hBackDC, Start_Pos + (note_width * 4) + 10, 450 - floor((double)((__int64)Bitmap.bmHeight * (HP)) / 1.5), Bitmap.bmWidth, floor(((__int64)Bitmap.bmHeight * (HP)) / 1.5), hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228));
	// 5*5 비트맵을 HP에 따라서 늘리고 줄인다.
	// 시작 지점과 이미지 길이를 조절해서 최대한 자연스럽게 꾸민다
	DeleteObject(hNewBitmap);

	if (PanVisualTimer > 0) { // 판정 이미지을 표시해야 할경우 (300ms 이내에 노트를 쳤을경우)
		if (LastPan == KOOL) { // 최근에 친 노트가 Kool 인경우
			hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_KOOL)); // Kool 이미지 로드
			GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hMemDC, hNewBitmap);
			GdiTransparentBlt(hBackDC, 238, 350, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // 그리기
			DeleteObject(hNewBitmap);
		}
		else if (LastPan == COOL) { // 이하 같음
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

	char buf[100]; // 표시할 버퍼 생성
	SetTextColor(hBackDC, RGB(255, 255, 255)); // 텍스트 색상 설정
	SetBkMode(hBackDC, TRANSPARENT); // 글자 배경색 투명

	if (Combo > 0) { // 콤보가 0 이상일경우 (콤보가 0일 경우 표시 안하기 위해)
		(HFONT)SelectObject(hBackDC, font_combo); // 폰트를 font_combo로 설정

		sprintf(buf, "%d", Combo); //콤보 buffer에 삽입
		RT_Change(&rt, 450, 50, 960, 160); // 글자 위치 변경
		DrawTextA(hBackDC, buf, -1, &rt, DT_CENTER); // 글자 드로우
	}

	(HFONT)SelectObject(hBackDC, font_score); // 스코어 폰트로 변경

	//이하 동일
	sprintf(buf, "%08d", Score); // 점수 표시
	RT_Change(&rt, 10, 10, 950, 160);
	DrawTextA(hBackDC, buf, -1, &rt, DT_RIGHT);

	(HFONT)SelectObject(hBackDC, font_acc);

	sprintf(buf, "%.2lf%%\n", Accurate); // 정확도 표시
	RT_Change(&rt, 10, 40, 950, 160);
	DrawTextA(hBackDC, buf, -1, &rt, DT_RIGHT);

	BitBlt(hDC, 0, 0, 1000, 500, hBackDC, 0, 0, SRCCOPY); // 백그라운드에서 그린 이미지 콘솔에 그리기

	DeleteObject(SelectObject(hBackDC, hBackBitmap)); // 사용한 오브젝트 정리
	DeleteObject(hNewBitmap);
	DeleteDC(hBackDC);
	DeleteDC(hMemDC);

	ReleaseDC(hWnd, hDC); // 윈도우 할당 해제
}

void CheckPlayStatus() //플레이 상태 체크
{
	if (PlayTimer >= Last_Note_pos + 5000) { // 노트가 끝났을 경우
		map_playing = FALSE; // 게임 끝내기
	}
}

void LoadMap(char* MapName) // 맵 불러오기
{
	char buf[500] = { NULL }; // 버퍼 선언
	if (LoadMapFile(MapName) == 0)
	{
		printf("Load Failed.");
	}
	sprintf(buf, "%s - %s (%s) [%s]", M_MetaData.Artist, M_MetaData.Title, M_MetaData.Creator, M_MetaData.Version);
	// 음악 제작자, 맵 이름, 맵 제작자, 버전 buffer에 쓰기
	SetConsoleTitle(buf); //버퍼 제목에 쓰기
}

void MusicLoad() // 음악 불러오기
{
	mciOpen.lpstrElementName = M_General.AudioFilename; // 맵에서 선택한 오디오 파일 불러오기
	mciOpen.lpstrDeviceType = "mpegvideo";

	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen); // 재생 커맨드 보내기

	dwID = mciOpen.wDeviceID; // 재생 디바이스 ID 설정
}

void MusicOpen(char* FileName) // 원하는 음악 불러오기
{
	mciOpen.lpstrElementName = FileName; // 파일 경로
	mciOpen.lpstrDeviceType = "mpegvideo";

	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen); // 재생 커맨드 보내기

	dwID = mciOpen.wDeviceID;
}

void MusicPlay(int ID, int repeat) // 음악 재생
{
	if (repeat == 0) // 반복하지 않을경우
		mciSendCommand(ID, MCI_PLAY, NULL, (DWORD)(LPVOID)&m_mciPlayParms); // 반복하지 않고 재생
	else
		mciSendCommand(ID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&m_mciPlayParms); // 음악이 끝나면 반복됨
}

void MusicPause(int ID) // 음악 일시정지
{
	mciSendCommand(ID, MCI_PAUSE, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay);
}

void MusicResume(int ID) // 음악 다시 재생
{
	mciSendCommandW(ID, MCI_RESUME, 0, NULL);
}

void MusicStop(int ID) // 음악 완전히 정지
{
	mciSendCommandW(ID, MCI_CLOSE, 0, NULL);
}

void SetMusicPosition(DWORD dwPosition) // 음악 플레이 위치 지정
{
	MCI_SEEK_PARMS mciSeekParams;
	mciSeekParams.dwTo = dwPosition;
	mciSendCommand(dwID, MCI_SEEK, MCI_TO, (DWORD)(LPVOID)&mciSeekParams); // 음악 위치 지정
}

void PlayMap() // 게임 플레이
{
	// 폰트 지정
	font = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Aller");
	font_combo = CreateFont(48, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Slant");
	font_score = CreateFont(36, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Slant");
	font_acc = CreateFont(32, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Cunia");

	char buf[500] = { NULL };

	// 스레드 생성
	TimerHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)M_Timer, 0, 0, NULL); // 타이머 스레드
	KeyPressHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)CheckKeyPress, 0, 0, NULL); // 키입력 스레드
	PanTimeHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)Pan_Timer, 0, 0, NULL); // 판정 표시 타이머 스레드
	GameHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)GameStatusCheck, 0, 0, NULL); // 전반적 게임 관리 스레드

	map_playing = TRUE; // 게임 플레이 시작
	while (map_playing) {
		CheckPlayStatus(); // 게임 상태 체크
		Render(); // 게임 화면 렌더링 시작
		if (exit_game == 1)
			break;
		//게임이 일시정지 됬을경우
		//if (IsPause) MusicPause(dwID); //음악 일시정지
		//else MusicResume(dwID);        //음악 재생
		while (IsPause) return;    // 일시정지시 함수 일시정지
	}
}

void ReadyRender() // 준비화면 렌더링
{
	// Render 함수와 중복되는 주석은 달지 않음
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
	(HFONT)SelectObject(hBackDC, font_score); // 폰트 설정

	SetTextColor(hBackDC, RGB(255, 255, 255)); // 색 설정
	SetBkMode(hBackDC, TRANSPARENT);

	sprintf(buf, "Map: %s [%s]", M_MetaData.Title, M_MetaData.Version); // 맵 이름, 버전 출력
	RT_Change(&rt, 20, 30, 900, 90);
	DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

	sprintf(buf, "Artist: %s", M_MetaData.Artist); // 음악 제작자 출력
	RT_Change(&rt, 20, 90, 700, 120);
	DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

	sprintf(buf, "Press [ENTER] To Start"); // 엔터누르면 시작 출력
	RT_Change(&rt, 20, 180, 700, 240);
	DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

	sprintf(buf, "Your Best Score : %d", UserDataLoad()); // 기존 최고 스코어 출력
	RT_Change(&rt, 20, 400, 700, 430);
	DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

	BitBlt(hDC, 0, 0, 1280, 720, hBackDC, 0, 0, SRCCOPY); // 이미지 콘솔에 그리기

	DeleteObject(SelectObject(hBackDC, hBackBitmap));
	DeleteDC(hBackDC);
	DeleteDC(hMemDC);

	ReleaseDC(hWnd, hDC);
}

void ReadyView() // 준비 뷰
{
	// 폰트 정의
	font_score = CreateFont(36, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Slant");

	SetMusicPosition(M_General.PreviewTime); //음악 시작시간을 프리뷰 타임에 맞춤
	MusicPlay(dwID, 1); // 음악 재생, 루프
	ReadyRender(); // 준비 렌더링 표시
	while (1)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x0001) { // 엔터키를 누르면 나가기
			break;
		}
	}

	//MusicPause(dwID); // 프리뷰 음악 재생 일시정지
	// 이 함수 이후 본 게임 렌더링 시작
}

void PauseView() // 일시정지 뷰
{

	//IsPause = 1; // 일시정지 활성화
	/**
	// Render 함수와 중복되는 주석은 달지 않음

	hWnd = GetConsoleWindow();
	hInst = GetModuleHandle(NULL);
	HDC hDC, hMemDC;
	static HDC hBackDC;
	HBITMAP hBackBitmap, hOldBitmap, hNewBitmap;
	BITMAP Bitmap;

	while (1) {
		hDC = GetDC(hWnd);

		hMemDC = CreateCompatibleDC(hDC);
		hBackDC = CreateCompatibleDC(hDC);

		hBackBitmap = CreateCompatibleBitmap(hDC, 1280, 720);
		hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);

		char buf[200];
		(HFONT)SelectObject(hBackDC, font_score);

		SetTextColor(hBackDC, RGB(255, 255, 255));
		SetBkMode(hBackDC, TRANSPARENT);

		sprintf(buf, "Pause..");
		RT_Change(&rt, 20, 30, 900, 90);
		DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

		sprintf(buf, "Press [ESC] to Resume");
		RT_Change(&rt, 20, 90, 900, 150);
		DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

		BitBlt(hDC, 0, 0, 1280, 720, hBackDC, 0, 0, SRCCOPY);

		DeleteObject(SelectObject(hBackDC, hBackBitmap));
		DeleteDC(hBackDC);
		DeleteDC(hMemDC);

		ReleaseDC(hWnd, hDC);

		if (GetAsyncKeyState(VK_ESCAPE) & 0x0001) { // [ESC] 키를 눌렀을 경우
			IsPause = 0; // 일시정지 해제
			break; //함수 나가기
		}
	}
	**/
	exit_game= 1;
	return; // 다시 원래 함수로 돌아감
}
void Reset() {
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
void Restart() // 재시작 함수
{
	// 변수 초기화
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
	LoadMap(NoteMapName);   // 맵 재로드
	MusicLoad();			// 음악 재로드

	SetMusicPosition(0);	// 음악 위치 원점으로
	MusicPlay(dwID, 0);     // 루프 없이 음악 재생
	map_playing = TRUE;		// 플레이 중 함수 활성화
	PlayMap();				// 맵 플레이
	Result();				// 결과 출력
}

void GameOverView() // 게임 오버 뷰
{
	// Render 함수와 중복되는 주석은 달지 않음
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
		if (GetAsyncKeyState(0x52) & 0x0001) {// [R] 키를 누를경우
			play_game_music();
			Restart();							// 재시작
			break;
		}
	}
}

void* ScoreIncVisual(int* Score_VS) // 스코어 부드럽게 올라가게 하기 (스레드 함수)
{
	for (int i = 0; i < Score; i++) // 스코어 만큼 반복
	{
		(*Score_VS)++;	// 스코어 ++
		if (i % ((Score / 1400) + 1) == 0) Sleep(2); //일정 초에만 Score_VS 출력
		// 곧, 일정 초가 되야만 글자가 반영이 되어 부드럽게 올라가는 연출이 된다
	}
}

void Result() // 결과 출력
{
	if (HP <= 0) { // HP가 0 이하, 죽어서 끝났을 경우
		MusicPause(dwID);	// 음악 종료
		GameOverView();		// 게임오버 뷰 출력
	}
	else { // 성공해서 깼을 경우
		HFONT fnt_score = CreateFont(36, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Cunia");
		HFONT fnt_result = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Cunia");
		HFONT fnt_combo = CreateFont(36, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Cunia");

		if (score[music] < Score) score[music] = Score; // 최고점수면 점수 기록
		char* buffs[200] = { NULL };
		sprintf(buffs, "{\"Kool\":%d,\"Cool\":%d,\"Good\":%d,\"Bad\":%d,\"Miss\":%d,\"MaxCombo\":%d,\"Acc\":%.2lf}"
			, TargetPass[KOOL], TargetPass[COOL], TargetPass[GOOD], TargetPass[BAD], TargetPass[MISS], MaxCombo, (double)Accurate);
		//send_score("player2", Score, M_MetaData.BeatmapID, buffs);
		//MusicOpen("applause.mp3");		// 결과화면 BGM 로드
		//MusicPlay(dwID, 0);		// 음악 재생

		int Score_View = 0; // 보이는 스코어 선언
		HANDLE IncHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ScoreIncVisual, &Score_View, 0, NULL);
		// 스코어 부드럽게 연출하기 위해 스레드 실행

	
		
		while (1) {
			// Render 함수와 중복되는 주석은 달지 않음
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
			GdiTransparentBlt(hBackDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // 그리기 580 0
			DeleteObject(hNewBitmap);

			hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_PASS));
			GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
			SelectObject(hMemDC, hNewBitmap);
			GdiTransparentBlt(hBackDC, 22, 400, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // 그리기 580 0
			DeleteObject(hNewBitmap);

			if (Score == Score_View) {
				if (Accurate >= 80.0) hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_RankS)); // S 판정 12
				else if (Accurate >= 70.0) hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_RankA)); // A 판정 15 
				else if (Accurate >= 60.0) hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_RankB)); // B 판정16
				else hNewBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP_RankC)); // C 판정 28
				GetObject(hNewBitmap, sizeof(BITMAP), &Bitmap);
				SelectObject(hMemDC, hNewBitmap);
				GdiTransparentBlt(hBackDC, 580, 10, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, RGB(255, 0, 228)); // 그리기 580 0
				DeleteObject(hNewBitmap);
			}

			sprintf(buf, "Music: %s ", M_MetaData.Title);
			RT_Change(&rt, 20, 30, 900, 90);
			DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

			sprintf(buf, "Score: %07d", Score_View); // 스코어 출력 (부드럽게)
			RT_Change(&rt, 20, 70, 700, 120);
			DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

			(HFONT)SelectObject(hBackDC, fnt_result);

			SetTextColor(hBackDC, RGB(200, 255, 255));
			SetBkMode(hBackDC, TRANSPARENT);

			sprintf(buf, "Kool: %d\n", TargetPass[KOOL]); //Kool, Cool... 출력
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

			sprintf(buf, "MaxCombo: %d\n", MaxCombo);	// 최대 콤보
			RT_Change(&rt, 20, 300, 500, 370);
			DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

			sprintf(buf, "Accuracy: %.2lf%%\n", (double)Accurate); // 정확도 출력
			RT_Change(&rt, 20, 350, 500, 400);
			DrawTextA(hBackDC, buf, -1, &rt, DT_LEFT);

			BitBlt(hDC, 0, 0, 1280, 720, hBackDC, 0, 0, SRCCOPY); // 이미지 표시

			DeleteObject(SelectObject(hBackDC, hBackBitmap));
			DeleteDC(hBackDC);
			DeleteDC(hMemDC);

			ReleaseDC(hWnd, hDC);
			if (GetAsyncKeyState(0x52) & 0x0001) {  // [R] 키를 누를경우
				break;
			}
		}
	}
}

/**
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
		if (GetAsyncKeyState(VK_UP) & 0x0001) { //위쪽 화살표 키를 눌렀을 때
			if (music_number == MAX_MUSIC)
				music_number = 1;
			else
				music_number++;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x0001) { //아래쪽 화살표 키를 눌렀을 때
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

