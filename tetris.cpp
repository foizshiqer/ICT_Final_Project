#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <sstream>
#include "Tetris_Class.h"

int main()
{
    // initialize
    tetrisClass tetris;

    std::string opLineString;

    while (!tetris.finished && getline(tetris.input, opLineString))
    {
        tetris.print();
        std::stringstream opLineSS(opLineString);
        while (!tetris.finished && opLineSS >> tetris.op)
        {
            tetris.operate();
        }

        if (tetris.finished)
        {
            break;
        }

        tetris.tick++;
        // should be at next tick
        if (!tetris.locked)
        {
            tetris.op = 'f';
            tetris.operate();
        }
        else
        {
            tetris.nextBlock();
        }
        // fall
        // no more tetrominos -> finish
        // fall -> newBlock if lock
    }

    // game over
    tetris.gameEnd();
}
// not like this : operation -> if fell -> next block -> no next -> end
// tick end / next tick -> fall -> if fell -> determine if die
// determine if fell only when tick-end fall

// no lock delay?
// try fall -> if fail -> lock