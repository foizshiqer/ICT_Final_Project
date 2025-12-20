#ifndef Tetris_Class
#define Tetris_Class

#include <iostream>
#include <fstream>
#include <queue>

class tetrisClass
{
public:
    tetrisClass();

    char board[20][10]; // 第一維 y (高度)，第二維 x (寬度)

    int score; // score

    char op; // operator A/D/R/F

    int tick;

    std::queue<char> seq; // block/tetrimino sequence

    char currentTetromino;

    bool gameover;

    bool finished;

    int cbx[4]; // idx 0 - 3 for 4 block in current tetromino

    int cby[4];

    std::ifstream input;

    std::ofstream output;

    std::ofstream perstep;

    int centerX;

    int centerY;

    bool locked;

    void nextBlock();

    void print(std::ofstream&); // can use ansi escape code to print colored text if nessesary

    void operate();

    void moveLeft(); // 'A'

    void moveRight(); // 'D'

    void rotate(); // 'R'

    void hardDrop(); // 'F', locks after this

    void fall(); // 1 tick, i.e. 1 line

    void calcScore();

    bool isfell();

    void lock(); // lock -> nextBlock

    void gameEnd();

private:
    // ____
    // TTT_
    // _T__
    // ____
    const int spawnXT[4] = {3, 4, 5, 4};
    const int spawnYT[4] = {1, 1, 1, 2};
    // ____
    // _L__
    // _L__
    // _LL_
    const int spawnXL[4] = {4, 4, 4, 5};
    const int spawnYL[4] = {1, 2, 3, 3};
    // ____
    // __J_
    // __J_
    // _JJ_
    const int spawnXJ[4] = {5, 5, 5, 4};
    const int spawnYJ[4] = {1, 2, 3, 3};
    // ____
    // ZZ__
    // _ZZ_
    // ____
    const int spawnXZ[4] = {3, 4, 4, 5};
    const int spawnYZ[4] = {1, 1, 2, 2};
    // ____
    // _SS_
    // SS__
    // ____
    const int spawnXS[4] = {3, 4, 4, 5};
    const int spawnYS[4] = {2, 2, 1, 1};
    // _I__
    // _I__
    // _I__
    // _I__
    const int spawnXI[4] = {4, 4, 4, 4};
    const int spawnYI[4] = {0, 1, 2, 3};
    // ____
    // _OO_
    // _OO_
    // ____
    const int spawnXO[4] = {4, 4, 5, 5};
    const int spawnYO[4] = {1, 1, 2, 2};
};

#endif