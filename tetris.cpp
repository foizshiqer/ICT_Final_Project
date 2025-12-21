#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <sstream>
#include "Tetris_Class.h"

enum class GameState // 把狀況全部存進state
{
    INPUT, // 讀 input 指令
    FALL,  // 自動下落
    LOCK,  // 方塊剛被鎖定
    END    // 遊戲結束
};

int main()
{
    tetrisClass tetris;
    GameState state = GameState::INPUT;

    tetris.print(tetris.perstep); // initial state

    while (state != GameState::END)
    {
        // 結束條件統一判斷
        if (tetris.shouldFinish())
        {
            state = GameState::END;
            break;
        }

        switch (state)
        {
        case GameState::INPUT:
        {
            // 嘗試讀一個 input 指令
            if (tetris.input.get(tetris.op))
            {
                // skip spaces
                while (tetris.op == ' ' && !tetris.input.eof())
                    tetris.input.get(tetris.op);

                if (tetris.op == '\n')
                {
                    state = GameState::FALL;
                    break;
                }

                tetris.operate();

                // F -> 該行結束
                if (tetris.op == 'F')
                {
                    while (tetris.op != '\n' && tetris.input.get(tetris.op))
                        ;
                    state = GameState::LOCK;
                }
            }
            else
            {
                // input 用完，進入自動下落
                state = GameState::FALL;
            }
            break;
        }

        case GameState::FALL:
        {
            tetris.tick++;
            tetris.op = 'f';
            tetris.operate();

            if (tetris.locked)
                state = GameState::LOCK;
            else
                state = GameState::INPUT;

            break;
        }

        case GameState::LOCK:
        {
            // LOCK後生成新方塊
            tetris.nextBlock();
            tetris.print(tetris.perstep);

            state = GameState::INPUT;
            break;
        }

        case GameState::END:
            break;
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