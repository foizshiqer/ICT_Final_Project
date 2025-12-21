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

    op = '0'; // no need

    // i/o
    input.open("input.txt");
    output.open("output.txt");
    perstep.open("output_detail.txt");

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
    input.get(); // \n = 10
    nextBlock();

    // dead
    gameover = false;

    // hold
    holding = false;
    holdCooldown = false;
}

void tetrisClass::print(std::ofstream &stream)
{
    stream << "SCORE: " << score << '\n';
    stream << "TIME: " << tick << '\n';
    stream << "OPERATION: " << op << '\n';
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
    angle = 0;
    if (seq.empty())
    {
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
        if (board[cby[i]][cbx[i]] != ' ') // failed to generate
        {
            gameover = true;
            return;
        }
    }
}

void tetrisClass::initTetromino()
{
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
        if (board[cby[i]][cbx[i]] != ' ') // failed to generate
        {
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

    case 'H':
        hold();
        break;

    case 'f':
        fall();
        break;

    default:
        std::cout << "Error operator: " << op << '\n';
    }
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
    print(perstep);
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
    print(perstep);
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
        break;

    default:
        break;
    }
    angle = (angle + 1) % 4;
    print(perstep);
}

void tetrisClass::hardDrop()
{
    while (true)
    {
        if (isfell())
            break;
        for (int idx = 0; idx < 4; idx++)
            cby[idx]++;
    }
    lock();
}

void tetrisClass::fall()
{
    if (isfell())
    {
        lock();
        return;
    }
    else
    {
        for (int idx = 0; idx < 4; idx++)
            cby[idx]++;
        print(perstep);
    }
}

void tetrisClass::hold()
{
    if (holdCooldown)
    {
        return;
    }

    holdCooldown = true;
    if (holding)
    {
        char temp = currentTetromino;
        currentTetromino = holdingTetromino;
        holdingTetromino = temp;
        initTetromino(); // 初始化 current 的位置
    }
    else
    {
        holding = true;
        holdingTetromino = currentTetromino;
        nextBlock(); // 取下一個方塊
    }
    print(perstep);
}

void tetrisClass::calcScore()
{
    int deleteLines = 0;
    for (int i = 19; i >= 0;)
    {
        bool canClear = true;
        for (int j = 0; j < 10; j++)
        {
            if (board[i][j] == ' ')
            {
                canClear = false;
                break;
            }
        }

        if (canClear)
        {
            if (deleteLines == 0)
            {
                print(perstep); // print board before clearing
            }

            for (int r = i; r > 0; r--)
            {
                for (int c = 0; c < 10; c++)
                {
                    board[r][c] = board[r - 1][c];
                }
            }
            for (int c = 0; c < 10; c++)
            {
                board[0][c] = ' ';
            }

            deleteLines++;
            // 重新檢查同一行
        }
        else
        {
            i--; // 往上
        }
    }

    switch (deleteLines)
    {
    case 0:
        break;
    case 1:
        score += 100;
        break;
    case 2:
        score += 300;
        break;
    case 3:
        score += 500;
        break;
    case 4:
        score += 800;
        break;
    default:
        break;
    }
}

bool tetrisClass::isfell()
{
    for (int idx = 0; idx < 4; idx++)
    {
        if (cby[idx] < 0 || cbx[idx] < 0 || cbx[idx] >= 10)
            return true;
        if (cby[idx] >= 19)
            return true;
        if (board[cby[idx] + 1][cbx[idx]] != ' ')
            return true;
    }
    return false;
}

void tetrisClass::lock()
{
    locked = true;
    holdCooldown = false;

    for (int i = 0; i < 4; i++)
        board[cby[i]][cbx[i]] = currentTetromino;

    print(perstep);

    calcScore();
    // should i put this here or in main -> if (!tetris.locked) else{ here } ? -> put in main
}

bool tetrisClass::shouldFinish()
{
    // 1. gameover
    if (gameover)
        return true;

    // 2. input 指令用完
    if (input.eof())
        return true;

    // 3. seq 用完 + 已 lock
    if (seq.empty() && locked)
        return true;

    return false;
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
