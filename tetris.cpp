#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include "Tetris_Class.h"

enum GameState // 把狀態存成 state, 然後就好了???
{
    INPUT,
    FALL,
    LOCK,
    END
};

int main()
{
    tetrisClass tetris;
    GameState state = INPUT;

    while (state != END)
    {
        // 結束條件統一判斷，不然會炸
        // 不要用 finished
        if (tetris.shouldFinish())
        {
            state = END;
            break;
        }

        switch (state)
        {
        case INPUT:
        {
            if (!tetris.input.get(tetris.op)) // 不然最後一個指令會執行兩次
            {
                state = END;
                break;
            }

            // skip spaces
            while (tetris.op == ' ' && !tetris.input.eof()) // end with R^X not R_, if R\n then will fall
            {
                tetris.input.get(tetris.op);
            }

            if (tetris.op == '\n') // 一行 input 讀完 -> fall
            {
                state = FALL;
                break;
            }

            tetris.operate();

            if (tetris.op != 'F')
            {
                // state = INPUT;
                break;
            }

            // F -> 該行結束，後面不管
            while (tetris.op != '\n' && tetris.input.get(tetris.op))
                ;
            state = LOCK;

            break;
        }

        case FALL:
        {
            tetris.op = 'f';
            tetris.operate();

            if (tetris.locked)
                state = LOCK;
            else
                state = INPUT;

            break;
        }

        case LOCK:
        {
            // LOCK 之後生成新方塊
            tetris.nextBlock();
            tetris.print(tetris.perstep);

            state = INPUT;
            break;
        }

        case END:
        {
            break;
        }

        default:
        {
            std::cout << "Error State: " << state << '\n';
            tetris.perstep << "Error State: " << state << '\n';
            state = END;
            break;
        }
        }
    }

    tetris.gameEnd();
    return 0;
}
// not like this : operation -> if fell -> next block -> no next -> end
// tick end / next tick -> fall -> if fell -> determine if die
// determine if fell only when tick-end fall

// no lock delay?
// try fall -> if fail -> lock

// prints perstep after operation