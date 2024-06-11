#pragma once
// генерация колоды карт
int** generateCardSet();
// отображение колоды
void show(int** array);
// перемешивание колоды
void shuffleCards(int** array);
// создание игроков (максимум 6 минимум 3)
string* createPlayers(int count);
// стартовая раздача денег
int* createCash(int countPlayers, int countMoney);
// отображение игроков на поле
void showPlayers(string*& players, int*& cash, int count);