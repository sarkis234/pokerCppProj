#include <iostream>
#define random(a,b) a + rand()% b + 1 - a
using namespace std;
#include "protoTypes.h"
#include "constants.h"



int** generateCardsSet() {


    int** arr = new int* [52] {};
    for (int value = 2, index = 0; value < 15; value++) {
        for (int suit = 3; suit < 7; suit++) {
            arr[index++] = new int[2] {value, suit};
        }
    }
    return arr;
}

void shuffleCardSet(int** arr) {
    int size = _msize(arr) / sizeof(arr[0]);
    for (int i = 0; i < 52; i++) {
        swap(arr[i], arr[random(0, size - 1)]);
    }
}

void showPlayer(string players, int cash, int**& playersSet) {
    cout << players << "\t" << cash << "$" << " [";
    show(playersSet);
    cout << "] " << endl;
}

int* createCash(int countPlayer, int countMoney) {
    int* cash = new int[countPlayer];
    for (int i = 0; i < countPlayer; i++) {
        cash[i] = countMoney;
    }
    return cash;
}

void showCards(int** arr) {
    int size1 = _msize(arr) / sizeof(arr[0]);

    for (int i = 0; i < size1; i++) {
        if (arr[i][0] == 11) {
            cout << "J" << char(arr[i][1]) << " ";
        }
        else if (arr[i][0] == 12) {
            cout << "Q" << char(arr[i][1]) << " ";
        }
        else if (arr[i][0] == 13) {
            cout << "K" << char(arr[i][1]) << " ";
        }
        else if (arr[i][0] == 14) {
            cout << "A" << char(arr[i][1]) << " ";
        }
        else {
            cout << arr[i][0] << char(arr[i][1]) << " ";
        }
    }
}

string* createPlayers(int count) {
    if (count > 6 || count < 3) {
        cout << "недостаточное кол-во игроков" << endl;
        return nullptr;
    }
    else {
        string* arr = new string[count];
        arr[0] = "Игрок";
        for (int i = 1, flag; i < count; i++) {
            arr[i] = NAMES[random(0, COUNT_NAMES - 1)];
            flag = false;
            for (int j = 1; j < i; j++) {
                if (arr[i] == arr[j]) {
                    flag = true;
                    break;
                }
            }
            if (flag) i--;
        }
        return arr;
    }

}

void showPlayers(string*& players, int* cash, int count) {
    cout << endl;
    for (int i = 0; i < count; i++) {
        cout << players[i] << "\t" << cash[i] << endl;
    }
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


int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "");
    int** mainSet = generateCardsSet();
    shuffleCardSet(mainSet);
    show(mainSet);
    int playersCount = 6;
    int* cash = createCash(playersCount, DEFAULT_CASH);
    string* playersName = createPlayers(playersCount);
    int*** playersSets = new int** [playersCount];

    for (int i = 0; i < playersCount; i++) {
        playersSets[i] = new int* [0];
    }

    for (int i = 0; i < playersCount; i++) {
        for (int j = 0; j < 2; j++) {
            transferTopCard(mainSet, playersSets[i]);
        }
    }

    for (int i = 0; i < playersCount; i++) {
        showPlayer(playersName[i], cash[i], playersSets[i]);
    }

    int** tableSet = new int* [0];
    for (int i = 0; i < 3; i++) {
        transferTopCard(mainSet, tableSet);
    }

    cout << "стол";
    showCards(tableSet);

    int blinde = DEFAULT_CASH/20;
    int smallBlinde = blinde / 2;
    int indexSmallBlinde = 0;
    int currentPlayer;
    while (true)
    {
        if (indexSmallBlinde > playersCount - 1) indexSmallBlinde = 0;
        currentPlayer = indexSmallBlinde;
        if (cash[indexSmallBlinde] >= smallBlinde) {
            cash[currentPlayer++] -= smallBlinde;
        }
        if (cash[indexSmallBlinde+1] >= blinde) {
            cash[currentPlayer++] -= blinde;
        }

        for (int i = indexSmallBlinde; i < playersCount; i++) {
            cout << "введите свою ставку: ";
            int currentBlinde;
            in >> currentBlinde;
            if (currentBlinde == 0) {
                cout << "игрок пасанул" << endl;
                continue;
            }
            else if (currentBlinde < blinde) {
                cout << "недостаточная ставка" << endl;
                i--;
                continue;
            }
            else if(currentBlinde < cash[i]){
                cout << "недостаточно денег" << endl;
                i--;
                continue;
            }
            else {
                cout << "ставка принята" << endl;
            }
        }
        for (int i = 0; i < indexSmallBlinde; i++) {

        }
    }

}
