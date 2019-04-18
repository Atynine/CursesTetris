//
// Created by rylan on 4/18/19.
//

#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include "tetromino.h"

#define BOARD_SIZE_X 10
#define BOARD_SIZE_Y 20
#define BOARD_OFFSET_X 36
#define BOARD_OFFSET_Y 1

class Game {
public:
    Game();
    void update(int delta);
    void render();
    int getSecondsPlayed();
    int getScore(){ return this->score; }
    bool hasEnded();
    bool hasQuit();
private:
    Tetromino* controlledTetromino = NULL;
    bool ended, quit;
    unsigned long timePlayed = 0, timeFromUpdate = 0, score = 0;//, updateInterval = 750000000;
    unsigned long updateInterval = 750000000;
    int grid[BOARD_SIZE_X][BOARD_SIZE_Y];

    void lockTetromino();
};


#endif //TETRIS_GAME_H
