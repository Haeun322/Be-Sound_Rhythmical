//���� ��� ȭ�鿡���� ���� ���õ� ����� ����ϴ� �ҽ�����
#include <Windows.h>
#include "calscore.h"
#include <process.h>

int Score = 0;
int PanVisualTimer = 0;

int TargetPass[5] = { 0 };
int LastPan = -1;

int M_NoFail = 0;

int HP = 100;
int Combo = 0;
int MaxCombo = 0;
double Rate = 1.0;
double HPUpRate = 1.0;
double HPDownRate = 1.0;
double Accurate = 100.0;

void CalcAcc() // ��Ȯ�� ���
{
	Accurate = (((__int64)((__int64)70 * TargetPass[BAD]) + ((__int64)130 * TargetPass[GOOD]) + ((__int64)200 * ((__int64)TargetPass[COOL] + (__int64)TargetPass[KOOL]))) /
		(((__int64)TargetPass[MISS] + TargetPass[BAD] + TargetPass[GOOD] + TargetPass[COOL] + TargetPass[KOOL]) * 2.0));
	// 100.00% ����
}

void* ScoreIncSoft(int a) // ���ھ� �ε巴�� ���� (������ �Լ�)
{
	for (int i = 0; i <= a; i++)
	{
		Score++;
		if ((i % ((a / 47) + 1)) == 0) Sleep(1); // ���ھ� �ε巴�� ������Ű��
	}
	return 0;
}

void IncScore(int s) // ���ھ� ����
{
	int sc = (s * Rate); // ���ھ� ���
	HANDLE IncHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)ScoreIncSoft, sc, 0, NULL); // �ε巴�� ������Ű�� ������ ����
}

void IncKool() // �� ����
{
	TargetPass[KOOL] ++;    // �� �Է� 
	LastPan = KOOL;			// ���� ǥ���Ұ� �Լ� ���
	PanVisualTimer = MaxVisualPanTime; // ���� ǥ�� �ð� �ʱ�ȭ
	Combo++;				// �޺� ++
	if (Combo >= MaxCombo) MaxCombo++;	// Combo�� MaxCombo���� ũ�� MaxCombo �߰�
	if (HP + (2.5 * HPUpRate) <= 100) HP += (2.5 * HPUpRate); // HP ȸ��
	else HP = 100;
	HPUpRate *= 1.15; // HP ��� ����ġ
	HPDownRate = 1.0; // HP �϶� ����ġ ����
	IncScore(55);	  // ���ھ� ����
	CalcAcc();
}

void IncCool() // Cool ����
{
	// �� �Լ��� �����κ� ����
	TargetPass[COOL]++;
	LastPan = COOL;
	PanVisualTimer = MaxVisualPanTime;
	Combo++;
	if (Combo >= MaxCombo) MaxCombo++;
	if (HP + (2 * HPUpRate) <= 100) HP += (2 * HPUpRate);
	else HP = 100;
	HPUpRate *= 1.1;
	HPDownRate = 1.0;
	IncScore(50);
	CalcAcc();
}

void IncGood()
{
	// �� �Լ��� �����κ� ����
	TargetPass[GOOD]++;
	LastPan = GOOD;
	PanVisualTimer = MaxVisualPanTime;
	Combo++;
	if (Combo >= MaxCombo) MaxCombo++;
	if (HP + (1 * HPUpRate) <= 100) HP += (1 * HPUpRate);
	else HP = 100;
	HPUpRate *= 1.05;
	HPDownRate = 1.0;
	IncScore(35);
	CalcAcc();
}

void IncBad()
{
	// �� �Լ��� �����κ� ����
	TargetPass[BAD]++;
	LastPan = BAD;
	PanVisualTimer = MaxVisualPanTime;
	Combo++;
	if (Combo >= MaxCombo) MaxCombo++;
	HP--;
	HPDownRate *= 1.1;
	IncScore(15);
	CalcAcc();
}

void IncMiss()
{
	// �� �Լ��� �����κ� ����
	TargetPass[MISS]++;
	LastPan = MISS;
	PanVisualTimer = MaxVisualPanTime;
	Combo = 0;		// �޺� �ʱ�ȭ
	HP -= (3 * HPDownRate); // HP ����
	HPDownRate *= 1.1; // HP ���� ����ġ
	HPUpRate = 1.0;
	CalcAcc();
}