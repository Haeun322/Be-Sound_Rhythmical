// main 함수 : 게임 시작 및 음악 선택, 게임시작, 게임 종료, 결과 출력등을 담당하고 있음 
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"winmm.lib")
#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <process.h>
#include "readmap.h"
#include "resource.h"
#include "Digitalv.h"
#include "calscore.h"
//#include "menu.h"
#include "gameplay.h"
int main(){ 
	SetConsoleTitle("Be_Sound_Rhythmical");
	while (1) {
		system("cls");
		restart = 0;
		start = 0;
		system("cls");
		
		print_title();
		loading();
		system("cls");
		if (mode == 0) {
			story();
			if (state == 7) {
				start_tutorial();
			}
		}
		else {
			while (start == 1) {
				load_score();
				system("cls");
				print_menu();
				//_beginthreadex(NULL, 0, input_key, 0, 0, NULL); 키 입력 스레드였으나 다시하기 기능 구현 과정 중 필요 없다고 판단되어 코드에서 제외되었다.
				play_preview_music();
				draw_icon();
				select_music();
				if (restart == 1) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					PlaySound(NULL, 0, 0);
					break;
				}
				else if (start == 2) {
					Reset();
					PlaySound(NULL, 0, 0);
					ClearCursor();
					system("cls");
					loading();
					system("cls");
					PlaySound(NULL, 0, 0);
					play_game_music();
					LoadMap(NoteMapName);
					map_playing = TRUE;
					PlayMap();
					system("cls");
					if (exit_game != 1) {
						Result();
						save_score();
					}
					else {
						PlaySound(NULL, 0, 0);
					}
					start = 1;
					exit_game = 0;
				}
			}
		}
	}
	return 0;
}