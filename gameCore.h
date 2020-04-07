#ifndef GAMECORE_H
#define GAMECORE_H
#include <vector>
using namespace std;

enum cellState {
    UNDIG,
    DIGGED,
    MARKED,
    WRONGMARKED,
    BOMB
};

struct cell {
    cellState curState;
    int value;
    cell(cellState cs = UNDIG, int val = 0) {
        curState = cs;
        value = val;
    }
};

enum gameState {
    PLAYING,
    FAUSE,
    WIN,
    OVER
};

enum gameLevel {
    BASIC,
    MEDIUM,
    HARD
};

const int mRow = 15;
const int mCol = 20;
const int mMineNum = 50;
const int mTime = 0;

class gameCore {
public:
    vector< vector<cell> > Map;
    int Row, Column, totalMineNum, curMineNum, Time;
    gameState gamestate;
    gameLevel gamelevel;
    gameCore(); 
    virtual ~gameCore();

    void digMine(int m,int n);
    void markMine(int m,int n);
    void createGame(int row = mRow, int col = mCol, int mineNum = mMineNum, gameLevel lv= MEDIUM);
    void restartGame();
    void checkGame();
};

#endif // GAMECORE_H
