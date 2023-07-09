//���� ���� ����µ� �Ҹ�Ǵ� �ð��� �����ϱ� ���� ���� osu! �������� �о�ͼ� ���� �÷��̿� ����
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readmap.h"

int NotePoints[MAX_TSTAMP][M_ROW] = { 0 };		// ��Ʈ Ŭ�� ǥ��
int ImagePoints[MAX_TSTAMP][M_ROW] = { 0 };		// ��Ʈ ǥ�ÿ�
int TPoint_array_section = 0;					// Ÿ�̹� ����Ʈ ��ġ
int Last_Note_pos = 0;							// ������ ��Ʈ�� �ð�
int _KEY_COUNT_ = 4;							// Ű ī��Ʈ (4Ű)

void TPoint(char* TStr)		// Ÿ�̹� ����Ʈ �о����
{
	int row[6] = { 0 };
	char buff[200] = { NULL };
	Trim(TStr, buff);		// �糡 �� ���� �����

	char* ptr = strtok(buff, ","); // ','�� �������� �ڸ���
	char strs[200];
	int i = 0, key = 0;
	while (ptr != NULL)               // �ڸ� ���ڿ��� ������ ���� ������ �ݺ�
	{
		Trim(ptr, strs);			  // �¿� ��ĭ ����
		row[i] = atoi(strs);          // �� �߰�
		ptr = strtok(NULL, ",");      // ���� ���ڿ��� �߶� �����͸� ��ȯ
		i++;
	}

	switch (row[0])					  // 0��° �� (Ű ��ġ)
	{
	case 64:	// 0��° Ű
		key = 0;
		break;
	case 192:	// 1��° Ű
		key = 1;
		break;
	case 320:	// 2��° Ű
		key = 2;
		break;
	case 448:	//3��° Ű
		key = 3;
		break;
	}

	NotePoints[row[2]][key] = 1;	  // ��Ʈ �߰�
	Last_Note_pos = row[2];			  // ������ ��Ʈ ��ġ ����
	if (row[3] == 128) {			  // �ճ�Ʈ ���� (0: �ܳ�Ʈ, 128: �ճ�Ʈ)
		for (int n = row[2]; n <= row[5]; n++) // �ճ�Ʈ ���������� ä��� : row[5] ����
			ImagePoints[n][key] = 2;  // �ճ�Ʈ ä���
	}
	else {
		ImagePoints[row[2]][key] = 1; // �ܳ�Ʈ ä���
	}
}

int Trim(char* line, char line2[]) // ��ĭ �ڸ���
{
	int len = 0;
	char cpTrim[MAX_STR_LEN];
	int xMan = 0;
	int i;

	len = strlen(line);				// ���� ���ڿ� ����
	if (len >= MAX_STR_LEN)
	{
		puts("string too long");
		return -1;
	}

	strcpy(cpTrim, line);			// ���ڿ� ����

	// �տ��� �߶󳻱�
	for (i = 0; i < len; i++)
	{
		if (cpTrim[i] == ' ' || cpTrim[i] == '\t')
			xMan++;
		else
			break;
	}

	// �ڿ��� �߶󳻱�
	for (i = len - 1; i >= 0; i--)
	{
		if (cpTrim[i] == ' ' || cpTrim[i] == '\t' || cpTrim[i] == '\n')
			cpTrim[i] = '\0';
		else
			break;
	}

	strcpy(line2, cpTrim + xMan);		// line2�� �ٿ� �ֱ�

	return strlen(line);
}

void ReadProperty_General(char* str)    // General Section �б�
{
	char nstr[200] = { NULL };
	char* ptr = strtok(str, ":");
	int i = 0;
	Trim(ptr, nstr);
	if (strcmp(nstr, "AudioFilename") == 0) {  // ����� ���� �̸�
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		strcpy(M_General.AudioFilename, nstr);
	}
	else if (strcmp(nstr, "AudioLeadIn") == 0) { // ����� ���� ��ġ
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		M_General.AudioLeadIn = atoi(ptr);
	}
	else if (strcmp(nstr, "PreviewTime") == 0) { // �̸����� ���� ��ġ
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		M_General.PreviewTime = atoi(ptr);
	}
	else if (strcmp(nstr, "Countdown") == 0) { // �ʹ� ī��Ʈ �ٿ� ����
		Trim(ptr, nstr);
		ptr = strtok(NULL, ":");
		M_General.Countdown = atoi(ptr);
	}
	else if (strcmp(nstr, "StackLeniency") == 0) { // �÷��� �����Ͻ�
		Trim(ptr, nstr);
		ptr = strtok(NULL, ":");
		M_General.StackLeniency = atof(ptr);
	}
}

void ReadProperty_MetaData(char* str)    // MetaData Section �б�
{
	char nstr[2000] = { NULL };
	char* ptr = strtok(str, ":");
	int i = 0;
	Trim(ptr, nstr);
	if (strcmp(nstr, "Title") == 0) {				// Ÿ��Ʋ ����
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		strcpy(M_MetaData.Title, nstr);
	}
	else if (strcmp(nstr, "TitleUnicode") == 0) {	// Ÿ��Ʋ ����
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		strcpy(M_MetaData.TitleUnicode, nstr);
	}
	else if (strcmp(nstr, "Artist") == 0) {			// ��Ƽ��Ʈ ����
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		strcpy(M_MetaData.Artist, nstr);
	}
	else if (strcmp(nstr, "ArtistUnicode") == 0) {	// ��Ƽ��Ʈ ����
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		strcpy(M_MetaData.ArtistUnicode, nstr);
	}
	else if (strcmp(nstr, "Creator") == 0) {		// ������
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		strcpy(M_MetaData.Creator, nstr);
	}
	else if (strcmp(nstr, "Version") == 0) {		// ���� �� �̸�
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		strcpy(M_MetaData.Version, nstr);
	}
	else if (strcmp(nstr, "Source") == 0) {			// �� �ҽ�
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		strcpy(M_MetaData.Source, nstr);
	}
	else if (strcmp(nstr, "Tags") == 0) {			// �� �±�
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		strcpy(M_MetaData.Tags, nstr);
	}
	else if (strcmp(nstr, "BeatmapID") == 0) {		// ��Ʈ�� ID
		ptr = strtok(NULL, ":");
		M_MetaData.BeatmapID = atoi(ptr);
	}
	else if (strcmp(nstr, "BeatmapSetID") == 0) {	// ��Ʈ�� �� ID
		ptr = strtok(NULL, ":");
		M_MetaData.BeatmapSetID = atoi(ptr);
	}
}

void ReadProperty_Difficulty(char* str)    // Difficulty Section �б�
{
	char nstr[200] = { NULL };
	char* ptr = strtok(str, ":");
	int i = 0;
	Trim(ptr, nstr);
	if (strcmp(nstr, "HPDrainRate") == 0) {				// HP ����
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		M_Difficulty.HPDrainRate = atof(nstr);
	}
	else if (strcmp(nstr, "CircleSize") == 0) {			// Ű ����
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		M_Difficulty.CircleSize = atof(nstr);
	}
	else if (strcmp(nstr, "OverallDifficulty") == 0) {	// ���� ���̵�1
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		M_Difficulty.OverallDifficulty = atof(nstr);
	}
	else if (strcmp(nstr, "ApproachRate") == 0) {		// ���� ���̵�2
		ptr = strtok(NULL, ":");
		Trim(ptr, nstr);
		M_Difficulty.ApproachRate = atof(nstr);
	}
}

void ReadProperty_TimingPoint(char* str)    // TimingPoint Section �б�
{
	//Structure [time,beatLength,meter,sampleSet,sampleIndex,volume,uninherited,effects]
	char* ptr = strtok(str, ",");
	char strs[200];
	int i = 0, key = 0;
	while (ptr != NULL)               // �ڸ� ���ڿ��� ������ ���� ������ �ݺ�
	{
		Trim(ptr, strs);			  // �¿� ��ĭ ����
		switch (i)
		{
		case 0:
			TimingPoints[TPoint_array_section].time = atoi(strs);			// ���� ���� �ð�
			break;
		case 2:
			TimingPoints[TPoint_array_section].meter = atoi(strs);			// ���� ����
			break;
		case 5:
			TimingPoints[TPoint_array_section].Volume = atoi(strs);			// ���� ����
			break;
		case 6:
			TimingPoints[TPoint_array_section].uninherited = atoi(strs);	// ��� ���� (BPM ���� or ��ũ�� �ӵ� ����)
			break;
		case 7:
			TimingPoints[TPoint_array_section].effects = atoi(strs);		// ȿ��
			break;
		case 1:
			TimingPoints[TPoint_array_section].beatLength = atof(strs);		// ��Ʈ ����
			break;
		}
		ptr = strtok(NULL, ",");      // ���� ���ڿ��� �߶� �����͸� ��ȯ
		i++;
	}
	TPoint_array_section++;
}

void ReadLine_Check(char* str, int section) {
	// Section �˻�

	switch (section)
	{
	case S_GENERAL:
		ReadProperty_General(str);		// �⺻ ����
		break;
	case S_EDITOR:
		// Editor (Non-Use)
		break;
	case S_METADATA:
		ReadProperty_MetaData(str);		// ��Ÿ ������ (����, �̸� ��)
		break;
	case S_DIFFICULTY:
		ReadProperty_Difficulty(str);	// ���̵� ����
		break;
	case S_EVENTS:
		// Events(ex. BG)
		break;
	case S_TIMINGPOINT:
		ReadProperty_TimingPoint(str);	// Ÿ�̹� ����
		break;
	case S_HOBJECT:
		TPoint(str);					// ��Ʈ ����
		break;
	}
}

int LoadMapFile(char* beatmap) // �� ���� �ҷ�����
{
	FILE* pFile = NULL;
	pFile = fopen(beatmap, "r");
	if (pFile != NULL)
	{
		char strTemp[MAX_STR_LEN];
		int section = 0;

		while (!feof(pFile))
		{
			fgets(strTemp, sizeof(strTemp), pFile);
			if (strcmp(strTemp, "[General]\n") == 0) { // �⺻����
				section = S_GENERAL;
			}
			else if (strcmp(strTemp, "[Editor]\n") == 0) { // ������
				section = S_EDITOR;
			}
			else if (strcmp(strTemp, "[Metadata]\n") == 0) { // ��Ÿ ������
				section = S_METADATA;
			}
			else if (strcmp(strTemp, "[Difficulty]\n") == 0) { // ���̵�
				section = S_DIFFICULTY;
			}
			else if (strcmp(strTemp, "[Events]\n") == 0) { // �̺�Ʈ
				section = S_EVENTS;
			}
			else if (strcmp(strTemp, "[TimingPoints]\n") == 0) { // Ÿ�̹� ����Ʈ
				section = S_TIMINGPOINT;
			}
			else if (strcmp(strTemp, "[HitObjects]\n") == 0) { // ��Ʈ
				section = S_HOBJECT;
			}
			else {
				ReadLine_Check(strTemp, section); // ���� �ҷ�����
			}
		}
		if (M_General.PreviewTime == -1)
			M_General.PreviewTime = 0;
		fclose(pFile); // ���� �ݱ�
		return 1;
	}
	else {
		return 0;
	}
}