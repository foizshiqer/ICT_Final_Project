#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <sstream>
#include "Tetris_Class.h"

enum GameState // 把狀況全部存進state
{
    INPUT, // 讀 input 指令
    FALL,  // 自動下落
    LOCK,  // 方塊剛被鎖定
    END    // 遊戲結束
};

int main()
{
    tetrisClass tetris;
    GameState state = INPUT;

    while (state != END)
    {
        // 結束條件統一判斷
        if (tetris.shouldFinish())
        {
            state = END;
            break;
        }

        switch (state)
        {
        case INPUT:
        {
            // 讀 input 指令
            tetris.input.get(tetris.op);

            // skip spaces
            while (tetris.op == ' ' && !tetris.input.eof()) // end with R^X not R_, if R\n then will fall
            {
                tetris.input.get(tetris.op);
            }

            if (tetris.op == '\n') // 一行 input 讀完 -> 下落
            {
                state = FALL;
                break;
            }

            tetris.operate();

            // F -> 該行結束，後面不管
            if (tetris.op == 'F')
            {
                while (tetris.op != '\n' && tetris.input.get(tetris.op))
                    ;
                state = LOCK;
            }

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
            // LOCK後生成新方塊
            tetris.nextBlock();
            tetris.print(tetris.perstep);

            state = INPUT;
            break;
        }

        case END:
            break;

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