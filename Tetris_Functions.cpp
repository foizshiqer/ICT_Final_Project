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

    // angle
    angle = 0;

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
    stream << "GAME STATUS: " << (gameover ? "GAME OVER\n" : "IN PROGRESS\n");
    stream << "BOARD:\n";

    for (int y = 0; y < 20; y++)
    {
        stream << '#';
        for (int x = 0; x < 10; x++)
        {
            char cell = board[y][x];
            bool isCurrent = false;

            // 只有在方塊還沒鎖定時才顯示 currentTetromino
            if (!locked)
            {
                for (int idx = 0; idx < 4; idx++)
                {
                    if (x == cbx[idx] && y == cby[idx])
                    {
                        isCurrent = true;
                        break;
                    }
                }
            }

            stream << (isCurrent ? currentTetromino : cell);
        }
        stream << "#\n";
    }
    stream << "############\n\n"; // 10 + 2 邊框
}

void tetrisClass::nextBlock()
{
    locked = false;
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

void tetrisClass::rotate()
{
    switch (currentTetromino)
    {
    case 'T':
        for (int i = 0; i < 4; i++)
        {
            if (cby[i] + rotateYT[angle][i] > 19 || cby[i] + rotateYT[angle][i] < 0 ||
                cbx[i] + rotateXT[angle][i] > 9 || cbx[i] + rotateXT[angle][i] < 0 ||
                board[cby[i] + rotateYT[angle][i]][cbx[i] + rotateXT[angle][i]] != ' ')
                return;
        }
        for (int i = 0; i < 4; i++)
        {
            cby[i] += rotateYT[angle][i];
            cbx[i] += rotateXT[angle][i];
        }
        break;

    case 'L':
        // Similar bounds + collision check
        for (int i = 0; i < 4; i++)
        {
            if (cby[i] + rotateYL[angle][i] > 19 || cby[i] + rotateYL[angle][i] < 0 ||
                cbx[i] + rotateXL[angle][i] > 9 || cbx[i] + rotateXL[angle][i] < 0 ||
                board[cby[i] + rotateYL[angle][i]][cbx[i] + rotateXL[angle][i]] != ' ')
                return;
        }
        for (int i = 0; i < 4; i++)
        {
            cby[i] += rotateYL[angle][i];
            cbx[i] += rotateXL[angle][i];
        }
        break;

    case 'J':
        for (int i = 0; i < 4; i++)
        {
            if (cby[i] + rotateYJ[angle][i] > 19 || cby[i] + rotateYJ[angle][i] < 0 ||
                cbx[i] + rotateXJ[angle][i] > 9 || cbx[i] + rotateXJ[angle][i] < 0 ||
                board[cby[i] + rotateYJ[angle][i]][cbx[i] + rotateXJ[angle][i]] != ' ')
                return;
        }
        for (int i = 0; i < 4; i++)
        {
            cby[i] += rotateYJ[angle][i];
            cbx[i] += rotateXJ[angle][i];
        }
        break;

    case 'Z':
        for (int i = 0; i < 4; i++)
        {
            if (cby[i] + rotateYZ[angle][i] > 19 || cby[i] + rotateYZ[angle][i] < 0 ||
                cbx[i] + rotateXZ[angle][i] > 9 || cbx[i] + rotateXZ[angle][i] < 0 ||
                board[cby[i] + rotateYZ[angle][i]][cbx[i] + rotateXZ[angle][i]] != ' ')
                return;
        }
        for (int i = 0; i < 4; i++)
        {
            cby[i] += rotateYZ[angle][i];
            cbx[i] += rotateXZ[angle][i];
        }
        break;

    case 'S':
        for (int i = 0; i < 4; i++)
        {
            if (cby[i] + rotateYS[angle][i] > 19 || cby[i] + rotateYS[angle][i] < 0 ||
                cbx[i] + rotateXS[angle][i] > 9 || cbx[i] + rotateXS[angle][i] < 0 ||
                board[cby[i] + rotateYS[angle][i]][cbx[i] + rotateXS[angle][i]] != ' ')
                return;
        }
        for (int i = 0; i < 4; i++)
        {
            cby[i] += rotateYS[angle][i];
            cbx[i] += rotateXS[angle][i];
        }
        break;

    case 'I':
        for (int i = 0; i < 4; i++)
        {
            if (cby[i] + rotateYI[angle][i] > 19 || cby[i] + rotateYI[angle][i] < 0 ||
                cbx[i] + rotateXI[angle][i] > 9 || cbx[i] + rotateXI[angle][i] < 0 ||
                board[cby[i] + rotateYI[angle][i]][cbx[i] + rotateXI[angle][i]] != ' ')
                return;
        }
        for (int i = 0; i < 4; i++)
        {
            cby[i] += rotateYI[angle][i];
            cbx[i] += rotateXI[angle][i];
        }
        break;

    case 'O':
        // Square block doesn’t rotate
        return;

    default:
        break;
    }
    angle = (angle + 1) % 4;
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
    if (gameover)
        print(perstep);
    input.close();
    output.close();
}
