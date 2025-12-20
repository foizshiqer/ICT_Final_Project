#ifndef Tetris_Functions
#define Tetris_Functions

#include <iostream>
#include <fstream>
#include <queue>
#include "Tetris_Class.h"

tetrisClass::tetrisClass()
{
    // 初始化棋盤
    for (int y = 0; y < 20; y++)
        for (int x = 0; x < 10; x++)
            board[y][x] = ' ';

    // score
    score = 0;

    // tick
    tick = 1;

    // i/o
    input.open("input.txt");
    output.open("output.txt");
    perstep.open("output_detail.txt");
    std::cout << "123456\n";

    // blockSeq
    // how to start?
    // use nextBlock
    int n;
    input >> n;
    for (int i = 0; i < n; i++)
    {
        char temp;
        input >> temp;
        seq.push(temp);
    }
    std::cout << (int)input.get();
    nextBlock();

    // dead
    gameover = false;
    finished = false;
}

void tetrisClass::print(std::ofstream &stream)
{
    stream << "SCORE: " << score << '\n';
    1 ? 'a' : 'b';
    stream << "GAME STATUS: ";
    if (gameover)
    {
        stream << "GAME OVER\n";
    }
    else
    {
        stream << "IN PROGRESS\n";
    }
    stream << "BOARD:\n";

    for (int y = 0; y < 20; y++)
    {
        stream << '#';
        for (int x = 0; x < 10; x++)
        {
            bool isCurrent = false;
            for (int idx = 0; idx < 4; idx++)
            {
                if (x == cbx[idx] && y == cby[idx])
                {
                    isCurrent = true;
                    break;
                }
            }
            stream << (isCurrent ? currentTetromino : board[y][x]);
        }
        stream << "#\n";
    }
    stream << "############\n\n"; // 10 + 2 邊框
}

void tetrisClass::nextBlock()
{
    if (seq.empty())
    {
        finished = true;
        return;
    }
    currentTetromino = seq.front();
    seq.pop();

    switch (currentTetromino)
    {
    case 'T':
        for (int i = 0; i < 4; i++)
        {
            cbx[i] = spawnXT[i];
            cby[i] = spawnYT[i];
        }
        break;
    case 'L':
        for (int i = 0; i < 4; i++)
        {
            cbx[i] = spawnXL[i];
            cby[i] = spawnYL[i];
        }
        break;
    case 'J':
        for (int i = 0; i < 4; i++)
        {
            cbx[i] = spawnXJ[i];
            cby[i] = spawnYJ[i];
        }
        break;
    case 'Z':
        for (int i = 0; i < 4; i++)
        {
            cbx[i] = spawnXZ[i];
            cby[i] = spawnYZ[i];
        }
        break;
    case 'S':
        for (int i = 0; i < 4; i++)
        {
            cbx[i] = spawnXS[i];
            cby[i] = spawnYS[i];
        }
        break;
    case 'I':
        for (int i = 0; i < 4; i++)
        {
            cbx[i] = spawnXI[i];
            cby[i] = spawnYI[i];
        }
        break;
    case 'O':
        for (int i = 0; i < 4; i++)
        {
            cbx[i] = spawnXO[i];
            cby[i] = spawnYO[i];
        }
        break;
    default:
        break;
    }
    for (int i = 0; i < 4; i++)
    {
        if (board[cbx[i]][cby[i]] != ' ') // failed to generate
        {
            finished = true;
            gameover = true;
            return;
        }
    }
}

void tetrisClass::operate()
{
    switch (op)
    {
    case 'A':
        moveLeft();
        break;

    case 'D':
        moveRight();
        break;

    case 'R':
        rotate();
        break;

    case 'F':
        hardDrop();
        break;

    case 'f':
        fall();
        break;

    default:
        output << "Error operator: " << op << '\n';
    }
    print(perstep);
}

void tetrisClass::moveLeft()
{
    for (int idx = 0; idx < 4; idx++)
    {
        if (cbx[idx] == 0 || board[cby[idx]][cbx[idx] - 1] != ' ')
            return;
    }
    for (int idx = 0; idx < 4; idx++)
        cbx[idx]--;
}

void tetrisClass::moveRight()
{
    for (int idx = 0; idx < 4; idx++)
    {
        if (cbx[idx] == 9 || board[cby[idx]][cbx[idx] + 1] != ' ')
            return;
    }
    for (int idx = 0; idx < 4; idx++)
        cbx[idx]++;
}

void tetrisClass::rotate() // how to rotate?
{
}

void tetrisClass::hardDrop()
{
    while (!isfell())
        fall();
    lock();
}

void tetrisClass::fall()
{
    if (isfell())
    {
        lock();
    }
    else
    {
        for (int idx = 0; idx < 4; idx++)
            cby[idx]++;
    }
}

void tetrisClass::calcScore()
{
}

bool tetrisClass::isfell()
{
    for (int idx = 0; idx < 4; idx++)
    {
        if (cby[idx] == 19 || board[cby[idx] + 1][cbx[idx]] != ' ')
            return true;
    }
    return false;
}

void tetrisClass::lock()
{
    locked = true;
    for (int i = 0; i < 4; i++)
        board[cby[i]][cbx[i]] = currentTetromino;
}

void tetrisClass::gameEnd()
{
    // maybe add some actions
    print(output);
    print(perstep);
    input.close();
    output.close();
}

#endif