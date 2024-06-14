#include <iostream>
#define random(a,b) a+rand()%(b+1-a)
using namespace std;

#include "prototypes.h"
#include "constants.h"

int** generateCardsSet() {
    //  suit - ♥3 : ♦4 : ♣5 : ♠6
    int** set = new int* [52];
    for (int index = 0, value = 2; value < 15; value++) {
        for (int suit = 3; suit < 7; suit++) {
            set[index++] = new int[2] {value, suit};
        }
    }
    return set;
}
void showCards(int** array) {
    int countCards = _msize(array) / sizeof(array[0]);
    for (int i = 0; i < countCards; i++) {
        switch (array[i][0]) {
        case 11:
            cout << "J";
            break;
        case 12:
            cout << "Q";
            break;
        case 13:
            cout << "K";
            break;
        case 14:
            cout << "A";
            break;
        default:
            cout << array[i][0];
        }
        cout << char(array[i][1]) << " ";
    }
}
void shuffle(int** array) {
    int size = _msize(array) / sizeof(array[0]);
    for (int i = 0; i < size; i++) {
        swap(array[i], array[random(0, size - 1)]);
    }
}
string* createPlayers(int count) {
    if (count > 6 || count < 1) {
        cout << "не верное количество игроков" << endl;
        return nullptr;
    }
    else {
        string* array = new string[count];
        array[0] = "Игрок";
        for (int i = 1, flag; i < count; i++) {
            array[i] = NAMES[random(0, COUNT_NAMES - 1)];
            flag = false;
            for (int j = 1; j < i; j++) {
                if (array[i] == array[j]) {
                    flag = true;
                    break;
                }
            }
            if (flag) i--;
        }
        return array;
    }

}
int* createCash(int countPlayers, int countMoney) {
    int* cash = new int[countPlayers];
    for (int i = 0; i < countPlayers; i++) {
        cash[i] = countMoney;
    }
    return cash;
}
void showPlayers(string*& players, int*& cash, int count) {
    for (int i = 0; i < count; i++) {
        cout << players[i] << "\t" << cash[i] << "$" << endl;
    }
}
void showPlayer(string players, int cash, int**& playersSet) {
    cout << players << "\t" << cash << "$" << " [ ";
    showCards(playersSet);
    cout << "]" << endl;
}
void transferTopCard(int**& outSet, int**& inSet) {
    int countOutSet = _msize(outSet) / sizeof(outSet[0]);
    int countInSet = _msize(inSet) / sizeof(inSet[0]);
    int** outSetBuf = new int* [countOutSet - 1];
    int** inSetBuf = new int* [countInSet + 1];
    for (int i = 0; i < countOutSet - 1; i++) {
        outSetBuf[i] = outSet[i];
    }
    for (int i = 0; i < countInSet; i++) {
        inSetBuf[i] = inSet[i];
    }
    inSetBuf[countInSet] = outSet[countOutSet - 1];
    delete[] inSet;
    delete[] outSet;
    inSet = inSetBuf;
    outSet = outSetBuf;
}
int maxElement(int* array) {
    int size = _msize(array) / sizeof(array[0]);
    int max = array[0];
    for (int i = 1; i < size; i++) {
        if (max < array[i]) max = array[i];
    }
    return max;
}
void resetCheckPlayers(bool* checkPlayers) {
    int size = _msize(checkPlayers);
    for (int i = 0; i < size; i++) {
        checkPlayers[i] = false;
    }
}
void resetPlayersSets(int***& playersSets) {
    int size = _msize(playersSets) / sizeof(playersSets[0]);
    for (int i = 0; i < size; i++) {
        //todo утечка памяти
        playersSets[i] = new int* [0];
    }
}

void startSettings(int**& mainSet, int& playersCount, string*& playersName, int*& cash) {
    srand(time(NULL));
    setlocale(LC_ALL, "");
    mainSet = generateCardsSet();
    shuffle(mainSet);
    while (true) {
        cout << "введите количество игроков от 2 до 6: ";
        cin >> playersCount;
        if (playersCount > 1 && playersCount < 7) break;
    }
    playersName = createPlayers(playersCount);
    cash = createCash(playersCount, DEFAULT_CASH);
}

void showGame(int**& mainSet, string*& playersName, int*& cash, int***& playersSets, int& playersCount, int**& tableSet) {
    system("cls");
    cout << "колода:" << endl;
    showCards(mainSet);
    cout << endl;
    for (int i = 0; i < playersCount; i++) {
        showPlayer(playersName[i], cash[i], playersSets[i]);
    }

    cout << "стол: ";
    showCards(tableSet);
}

void startGame(int playersCount, string*& playersName, int*& cash, int**& mainSet) {
    int*** playersSets = new int** [playersCount]; // колоды игроков
    resetPlayersSets(playersSets);
    //первичная раздача игрокам
    for (int i = 0; i < playersCount; i++) {
        for (int j = 0; j < 2; j++) {
            transferTopCard(mainSet, playersSets[i]);
        }
    }
    //вскрываем три карты на стол
    int** tableSet = new int* [0];
    for (int i = 0; i < 3; i++) {
        transferTopCard(mainSet, tableSet);
    }

    showGame(mainSet, playersName, cash, playersSets, playersCount, tableSet);
    cout << endl;

    int blinde = DEFAULT_CASH / 20;
    int smallBlinde = blinde / 2;
    int indexSmallBlinde = 0;
    int currentPlayer;

    int* currentBlinde = new int[playersCount];
    bool* checkPlayers = new bool[playersCount];
    resetCheckPlayers(checkPlayers);


    while (true)
    {
        if (indexSmallBlinde > playersCount - 1) {
            indexSmallBlinde = 0;
        }
        currentPlayer = indexSmallBlinde;

        if (cash[indexSmallBlinde] >= smallBlinde) {
            cash[currentPlayer++] -= smallBlinde;
        }

        if (cash[indexSmallBlinde + 1] >= blinde) {
            cash[currentPlayer++] -= blinde;
        }

        for (int i = indexSmallBlinde; i < playersCount; i++) {
            if (checkPlayers[i]) {
                i++;
                continue;
            }
            cout << playersName[i] << ", ";
            cout << "введите ставку: ";
            cin >> currentBlinde[i];
            if (currentBlinde[i] == 0) {
                cout << "игрок пасанул: " << endl;
                checkPlayers[i] = true;
                continue;
            }
            else if (currentBlinde[i] < blinde) {
                cout << "нужна ставка больше: " << endl;
                i--;
                continue;
            }
            else if (currentBlinde[i] > cash[i]) {
                cout << "недостаточно денег: " << endl;
                i--;
                continue;
            }
            else {
                cout << "ставка принята: " << endl;
                cash[i] -= currentBlinde[i];
            }
        }

        for (int i = 0; i < indexSmallBlinde; i++) {
            if (checkPlayers[i]) {
                i++;
                continue;
            }
            cout << "введите ставку: ";
            cin >> currentBlinde[i];
            if (currentBlinde[i] == 0) {
                cout << "игрок пасанул: " << endl;
                continue;
            }
            else if (currentBlinde[i] < blinde) {
                cout << "нужна ставка больше: " << endl;
                i--;
                continue;
            }
            else if (currentBlinde[i] > cash[i]) {
                cout << "недостаточно денег: " << endl;
                i--;
                continue;
            }
            else {
                cout << "ставка принята: " << endl;
                cash[i] -= currentBlinde[i];
            }
        }

        for (int i = 0; i < playersCount; i++) {
            showPlayer(playersName[i], cash[i], playersSets[i]);
            cout << "ставка текущая: " << currentBlinde[i] << endl;
        }

        int maxBlinde = maxElement(cash);
        for (int i = indexSmallBlinde; i < playersCount; i++) {
            if (checkPlayers[i]) {
                i++;
                continue;
            }
            if (currentBlinde[i] < maxBlinde) {
                int difBlinde = maxBlinde - currentBlinde[i];
                if (cash[i] < difBlinde) {
                    //todo реализовать возможность продолжения игры.
                    cout << "Недостаточно денег." << endl;
                    checkPlayers[i] = true;
                    continue;
                }
                else {
                    cash[i] -= difBlinde;
                }
            }
        }
        for (int i = 0; i < indexSmallBlinde; i++) {
            if (checkPlayers[i]) {
                i++;
                continue;
            }
            if (currentBlinde[i] < maxBlinde) {
                int difBlinde = maxBlinde - currentBlinde[i];
                if (cash[i] < difBlinde) {
                    //todo реализовать возможность продолжения игры.
                    cout << "Недостаточно денег." << endl;
                    checkPlayers[i] = true;
                    continue;
                }
                else {
                    cash[i] -= difBlinde;
                }
            }
        }
        cout << "после уравнивания ставок:" << endl;

        for (int i = 0; i < playersCount; i++) {
            showPlayer(playersName[i], cash[i], playersSets[i]);
            cout << "ставка текущая: " << currentBlinde[i] << " ";
            cout << (checkPlayers[i]) ? "пасанул" : "";
            cout << endl;
        }


    }

}

int main()
{
    int** mainSet = nullptr; // главная колода карт
    int playersCount; // количество игроков в игре (от 2 до 6)
    string* playersName = nullptr;
    int* cash = nullptr;
    startSettings(mainSet, playersCount, playersName, cash);
    startGame(playersCount, playersName, cash, mainSet);






}
 
