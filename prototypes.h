#pragma once
// ��������� ������ ����
int** generateCardSet();
// ����������� ������
void show(int** array);
// ������������� ������
void shuffleCards(int** array);
// �������� ������� (�������� 6 ������� 3)
string* createPlayers(int count);
// ��������� ������� �����
int* createCash(int countPlayers, int countMoney);
// ����������� ������� �� ����
void showPlayers(string*& players, int*& cash, int count);