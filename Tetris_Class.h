#include <iostream>
#include <fstream>
#include <queue>

class tetrisClass
{
public:
    tetrisClass();

    char board[20][10]; // 第一維 y (高度)，第二維 x (寬度)

    int score; // score

    char op; // operator A/D/R/F and H

    int tick;

    std::queue<char> seq; // block/tetrimino sequence

    char currentTetromino;

    char holdingTetromino;

    bool gameover;

    int cbx[4]; // idx 0 - 3 for 4 block in current tetromino

    int cby[4];

    std::ifstream input;

    std::ofstream output;

    std::ofstream perstep;

    int centerX;

    int centerY;

    int angle;

    bool locked;

    bool holdCooldown;

    bool holding;

    void nextBlock();

    void initTetromino();

    void print(std::ofstream &); // can use ansi escape code to print colored text if nessesary

    void operate(); // hold is not included, only A/D/R/F and f

    void moveLeft(); // 'A'

    void moveRight(); // 'D'

    void rotate(); // 'R'

    void hardDrop(); // 'F', locks after this

    void fall(); // 1 tick, i.e. 1 line

    void hold();

    void calcScore();

    bool isfell();

    void lock(); // lock -> nextBlock

    bool shouldFinish();

    void gameEnd();

private:
    // ____  __0_  ____  ____
    // 012_  _31_  __3_  _2__
    // _3__  __2_  _210  _13_
    // ____  ____  ____  _0__
    const int spawnXT[4] = {3, 4, 5, 4};
    const int spawnYT[4] = {1, 1, 1, 2};
    // rotate[angle][idx]
    const int rotateXT[4][4] = {
        {2, 1, 0, 0},
        {1, 0, -1, 1},
        {-2, -1, 0, 0},
        {-1, 0, 1, -1}};
    const int rotateYT[4][4] = {
        {-1, 0, 1, -1},
        {2, 1, 0, 0},
        {1, 0, -1, 1},
        {-2, -1, 0, 0}};

    // ____  ____  _32_  ____
    // _0__  210_  __1_  ___3
    // _1__  3___  __0_  _012
    // _23_  ____  ____  ____
    const int spawnXL[4] = {4, 4, 4, 5};
    const int spawnYL[4] = {1, 2, 3, 3};
    const int rotateXL[4][4] = {
        {1, 0, -1, -2}, // 0 -> 1
        {0, 1, 2, 1},  // 1 -> 2
        {-1, 0, 1, 2},  // 2 -> 3
        {0, -1, -2, -1}   // 3 -> 0
    };
    const int rotateYL[4][4] = {
        {0, -1, -2, -1},  // 0 -> 1
        {1, 0, -1, -2}, // 1 -> 2
        {0, 1, 2, 1},  // 2 -> 3
        {-1, 0, 1, 2}   // 3 -> 0
    };

    // ____  ____  _32_  ____
    // __0_  2___  _1__  _013
    // __1_  310_  _0__  ___2
    // _23_  ____  ____  ____
    const int spawnXJ[4] = {5, 5, 4, 5};
    const int spawnYJ[4] = {1, 2, 3, 3};
    const int rotateXJ[4][4] = {
        {0, -1, -1, -2},
        {-1, 0, 2, 1},
        {0, 1, 1, 2},
        {1, 0, -2, -1}};
    const int rotateYJ[4][4] = {
        {1, 0, -2, -1},
        {0, -1, -1, -2},
        {-1, 0, 2, 1},
        {0, 1, 1, 2}};

    // ____  __0_  ____  ____
    // 01__  _21_  _32_  __3_
    // _23_  _3__  __10  _12_
    // ____  ____  ____  _0__
    const int spawnXZ[4] = {3, 4, 4, 5};
    const int spawnYZ[4] = {1, 1, 2, 2};
    const int rotateXZ[4][4] = {
        {2, 1, 0, -1},
        {1, 0, 1, 0},
        {-2, -1, 0, 1},
        {-1, 0, -1, 0}};
    const int rotateYZ[4][4] = {
        {-1, 0, -1, 0},
        {2, 1, 0, -1},
        {1, 0, 1, 0},
        {-2, -1, 0, 1}};

    // ____  _0__  ____  ____
    // _23_  _12_  __10  _3__
    // 01__  __3_  _32_  _21_
    // ____  ____  ____  __0_
    const int spawnXS[4] = {3, 4, 4, 5};
    const int spawnYS[4] = {2, 2, 1, 1};
    const int rotateXS[4][4] = {
        {1, 0, 1, 0},
        {2, 1, 0, -1},
        {-1, 0, -1, 0},
        {-2, -1, 0, 1}};
    const int rotateYS[4][4] = {
        {-2, -1, 0, 1},
        {1, 0, 1, 0},
        {2, 1, 0, -1},
        {-1, 0, -1, 0}};

    // _0__  ____  __3_  ____
    // _1__  3210  __2_  ____
    // _2__  ____  __1_  0123
    // _3__  ____  __0_  ____
    const int spawnXI[4] = {4, 4, 4, 4};
    const int spawnYI[4] = {0, 1, 2, 3};
    const int rotateXI[4][4] = {
        {2, 1, 0, -1},
        {-1, 0, 1, 2},
        {-2, -1, 0, 1},
        {1, 0, -1, -2}};
    const int rotateYI[4][4] = {
        {1, 0, -1, -2},
        {2, 1, 0, -1},
        {-1, 0, 1, 2},
        {-2, -1, 0, 1}};

    // ____
    // _01_
    // _32_
    // ____
    const int spawnXO[4] = {4, 5, 5, 4};
    const int spawnYO[4] = {1, 1, 2, 2};
    // no rotation for O
};