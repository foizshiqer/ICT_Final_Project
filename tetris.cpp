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

    tetris.print(tetris.perstep); // detail, initial status
    while (!tetris.finished)
    {
        while (!tetris.finished && tetris.input.get(tetris.op))
        {
            while (tetris.op == ' ' && !tetris.input.eof())
            {
                tetris.input.get(tetris.op);
            }
            if (tetris.input.eof())
            {
                tetris.finished = true;
                break;
            }
            if (tetris.op == '\n')
            {
                break;
            }
            tetris.operate();
            if (tetris.op == 'F')
            {
                while (tetris.op != '\n')
                    tetris.input.get(tetris.op);
                break;
            }
        }

        if (tetris.input.eof())
        {
            tetris.finished = true;
            break;
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
            tetris.operate(); // fall
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

// prints perstep after operation