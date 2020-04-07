#include "gameCore.h"
#include <vector>
#include <time.h>
#include <stdlib.h>
using namespace std;

const int nxt[][2] = { {-1,-1},{-1,0},{-1,1},
                 {0,-1},{0,1},
                 {1,-1},{1,0},{1,1} };

gameCore::gameCore() {
    Row = mRow; //前缀带有m的都是已设定好的默认值
    Column = mCol;
    totalMineNum = mMineNum;
    Time = mTime;
    gamestate = PLAYING;
}

void gameCore::createGame(int row, int col, int mineNum, gameLevel lv) {
    Map.clear();
    Row = row;
    Column = col;
    totalMineNum = mineNum;
    curMineNum = mineNum;
    gamestate = PLAYING;
    gamelevel = lv;
    Time = 0;

    for (int i = 0; i < Row; i++) {
        vector<cell> lineCell;
        for (int j = 0; j < Column; j++) {
            lineCell.push_back(cell());
        }
        Map.push_back(lineCell);
    }

    srand((unsigned)time(nullptr));
    int k = totalMineNum;
    while (k) {
        int r = rand() % Row;
        int c = rand() % Column;
        if (Map[r][c].value != -1) {
            Map[r][c].value = -1;
            k--;
        }
    }

    for (int i = 0; i < Row; i++) {
        for (int j = 0; j < Column; j++) {
            if (Map[i][j].value != -1) {
                for (int k = 0; k < 8; k++) {
                    int tx = i + nxt[k][0];
                    int ty = j + nxt[k][1];
                    if (tx >= 0 && ty >= 0 && tx < Row && ty < Column) {
                        if(Map[tx][ty].value == -1) {
                            Map[i][j].value ++;
                        }
                    }
                }
            }
        }
    }
}

void gameCore::restartGame() {
    createGame(Row,Column,totalMineNum,gamelevel);
}

void gameCore::digMine(int m,int n) {
    if (Map[m][n].value > 0 && Map[m][n].curState == UNDIG) {
        Map[m][n].curState = DIGGED;
    }

    if (Map[m][n].value == 0 && Map[m][n].curState == UNDIG) {
        Map[m][n].curState = DIGGED;
        for (int i = 0; i < 8; i++) {
            int tx = m + nxt[i][0];
            int ty = n + nxt[i][1];
            if (tx >= 0 && ty >= 0 && tx < Row && ty < Column) {
                digMine(tx,ty);
            }
        }
    }

    if (Map[m][n].value == -1) {
        gamestate = OVER;
        Map[m][n].curState = BOMB;
    }

    checkGame();
}

void gameCore::markMine(int m, int n) {
    if (Map[m][n].curState == UNDIG) {
        if (Map[m][n].value == -1) {
            Map[m][n].curState = MARKED;
        } else {
            gamestate = FAUSE;
            Map[m][n].curState = WRONGMARKED;
        }
        curMineNum--;
    }
    else if(Map[m][n].curState == MARKED || Map[m][n].curState == WRONGMARKED) {
        Map[m][n].curState = UNDIG;
        bool tag = true;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (Map[i][j].curState == WRONGMARKED) {
                    tag = false;
                    break;
                }
            }
        }
        if(!tag) {
            gamestate = FAUSE;
        } else {
            gamestate = PLAYING;
        }
        curMineNum ++;
    }
    checkGame();
}

void gameCore::checkGame() {
    if (gamestate == OVER) {
        for (int i = 0; i < Row; i++) {
            for (int j = 0; j < Column; j++) {
                if (Map[i][j].value == -1) {
                    Map[i][j].curState = BOMB;
                }
            }
        }
        return ;
    }

    if (gamestate != FAUSE) {
        for (int i = 0; i < Row; i++) {
            for (int j = 0; j < Column; j++) {
                if (Map[i][j].curState == UNDIG) {
                    gamestate = PLAYING;
                    return ;
                }
            }
        }
        gamestate = WIN;
    }
}

gameCore::~gameCore() {

}
