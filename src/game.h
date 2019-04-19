//
// Created by rylan on 4/18/19.
//

#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include "tetromino.h"

#define BOARD_SIZE_X 11
#define BOARD_SIZE_Y 20
#define BOARD_OFFSET_X 36
#define BOARD_OFFSET_Y 1
#define LINES_PER_LEVEL 1

class Game {
public:
    Game();
    ~Game();
    void update(int delta);
    void render();
    int getSecondsPlayed();
    int getScore(){ return this->score; }
    int getLevel(){ return this->level; }
    bool hasEnded();
    bool hasQuit();
private:
    Tetromino* controlledTetromino = NULL;
    Tetromino* nextTetromino = NULL;
    Tetromino* heldTetromino = NULL;
    bool ended, quit;
    int level = 0, linesThisLevel = 0;
    unsigned long timePlayed = 0, timeFromUpdate = 0, score = 0, updateInterval = 750000000;
    int grid[BOARD_SIZE_X][BOARD_SIZE_Y];

    void lockTetromino();
    void updateScore(int numLines);
    void swapHeld();
    void getLevelSpeed();
};


#endif //TETRIS_GAME_H
