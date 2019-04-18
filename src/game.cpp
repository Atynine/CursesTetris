//
// Created by rylan on 4/18/19.
//

#include <curses.h>
#include <string.h>
#include "game.h"




Game::Game() {
    this->ended = this->quit = false;
    this->controlledTetromino = new Tetromino();
    memset(this->grid, 0, sizeof(this->grid));
}

void Game::update(int delta) {
    //Update time variables
    this->timePlayed += delta;
    this->timeFromUpdate+=delta;

    char i = getch();
    int prevSpeed = this->updateInterval;
    switch(i){
        case 'f':
            this->quit = true;
            break;
        case 'q':
            this->controlledTetromino->rotateLeft();
            return;
        case 'e':
            this->controlledTetromino->rotateRight();
            return;
        case 'a':
            this->controlledTetromino->moveLeft();
            return;
        case 'd':
            this->controlledTetromino->moveRight();
            return;
        case 's':
            this->updateInterval = this->updateInterval/4;
        default:
            break;
    }

    if(this->timeFromUpdate>this->updateInterval){
        if(this->updateInterval != prevSpeed) this->updateInterval = prevSpeed;
        this->timeFromUpdate = 0;
        int tetrX = this->controlledTetromino->getX();
        int tetrY = this->controlledTetromino->getY();
        if(tetrY + this->controlledTetromino->getSizeY() >= BOARD_SIZE_Y ||
            tetrX + this->controlledTetromino->getSizeX() >= BOARD_SIZE_X){
            this->lockTetromino();
        }else{
            for(int x = 0; x < this->controlledTetromino->getSizeX(); x++){
                if(this->grid[tetrX+x][tetrY + this->controlledTetromino->getSizeY()] &&
                    this->controlledTetromino->contains(tetrX+x, tetrY+this->controlledTetromino->getSizeY()-1)){
                    this->lockTetromino();
                    return;
                }
            }
        }

        this->controlledTetromino->update();
    }
    if(this->updateInterval != prevSpeed) this->updateInterval = prevSpeed;
}

void Game::render() {
    clear();
    for(int y = 0; y < BOARD_SIZE_Y; y++){
        for(int x = 0; x < BOARD_SIZE_X; x++){
            attron(COLOR_PAIR(this->grid[x][y]));
            mvprintw(y + BOARD_OFFSET_Y, x + BOARD_OFFSET_X, " ");
            attroff(COLOR_PAIR(this->grid[x][y]));
        }
    }

    attron(COLOR_PAIR(COLOR_WHITE));
    for(int y = 0; y < BOARD_SIZE_Y; y++){
        mvprintw(y + BOARD_OFFSET_Y, BOARD_OFFSET_X-1, " ");
        mvprintw(y + BOARD_OFFSET_Y, BOARD_OFFSET_X+BOARD_SIZE_X-1, " ");
    }
    for(int x = 0; x <= BOARD_SIZE_X; x++){
        mvprintw(BOARD_OFFSET_Y-1, x+BOARD_OFFSET_X-1, " ");
        mvprintw(BOARD_OFFSET_Y+BOARD_SIZE_Y, x+BOARD_OFFSET_X-1, " ");
    }
    attroff(COLOR_PAIR(COLOR_WHITE));

    this->controlledTetromino->render();

    mvprintw(1, BOARD_OFFSET_X+BOARD_SIZE_X+1, "TIME:\t %d", this->getSecondsPlayed());
    mvprintw(2, BOARD_OFFSET_X+BOARD_SIZE_X+1, "SCORE:\t %d", this->getScore());
}

void Game::lockTetromino() {
    int tetrX = this->controlledTetromino->getX();
    int tetrY = this->controlledTetromino->getY();
    if(tetrY == 0) this->ended = true;
    for(int x = 0; x < this->controlledTetromino->getSizeX(); x++){
        for(int y = 0; y < this->controlledTetromino->getSizeY(); y++){
            if(this->controlledTetromino->contains(tetrX+x, tetrY+y)){
                this->grid[tetrX+x][tetrY+y] = this->controlledTetromino->getColor();
            }
        }
    }
    delete this->controlledTetromino;
    this->controlledTetromino = new Tetromino();
}

bool Game::hasEnded() {
    return this->ended;
}

bool Game::hasQuit() {
    return this->quit;
}

int Game::getSecondsPlayed() {
    return this->timePlayed/1000000000;
}


