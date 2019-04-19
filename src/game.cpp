//
// Created by rylan on 4/18/19.
//

#include <curses.h>
#include <string.h>
#include "game.h"




Game::Game() {
    this->ended = this->quit = false;
    this->controlledTetromino = new Tetromino();
    this->nextTetromino = new Tetromino();
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
            this->controlledTetromino->rotateLeft(this->grid);
            return;
        case 'e':
            this->controlledTetromino->rotateRight(this->grid);
            return;
        case 'a':
            this->controlledTetromino->moveLeft(this->grid);
            return;
        case 'd':
            this->controlledTetromino->moveRight(this->grid);
            return;
        case 's':
            this->updateInterval = this->updateInterval/4;
            break;
        case 'w':
            this->swapHeld();
            return;
    }

    if(this->timeFromUpdate>=this->updateInterval){
        this->controlledTetromino->update();
        if(this->updateInterval != prevSpeed) this->updateInterval = prevSpeed;
        this->timeFromUpdate = 0;
        int tetrX = this->controlledTetromino->getX();
        int tetrY = this->controlledTetromino->getY();
        if(tetrY + this->controlledTetromino->getSizeY() >= BOARD_SIZE_Y ||
            tetrX + this->controlledTetromino->getSizeX() >= BOARD_SIZE_X){
            this->lockTetromino();
        }else if(this->controlledTetromino->isIntersecting(this->grid)){
            this->controlledTetromino->moveUp();
            this->lockTetromino();
        }
        return;
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

    this->controlledTetromino->render(BOARD_OFFSET_X, BOARD_OFFSET_Y);
    this->nextTetromino->render(BOARD_OFFSET_X+BOARD_SIZE_X, (BOARD_SIZE_Y-this->nextTetromino->getSizeY())/2+BOARD_OFFSET_Y);
    if(this->heldTetromino != NULL){
        this->heldTetromino->render(BOARD_OFFSET_X+BOARD_SIZE_X, (BOARD_SIZE_Y-this->nextTetromino->getSizeY())/2+BOARD_OFFSET_Y+5);
        mvprintw((BOARD_SIZE_Y-this->nextTetromino->getSizeY())/2+BOARD_OFFSET_Y+4, BOARD_OFFSET_X+BOARD_SIZE_X+3, "HELD");
    }

    mvprintw((BOARD_SIZE_Y-this->nextTetromino->getSizeY())/2+BOARD_OFFSET_Y-1, BOARD_OFFSET_X+BOARD_SIZE_X+3, "NEXT");
    mvprintw(1, BOARD_OFFSET_X+BOARD_SIZE_X+1, "TIME:\t %d", this->getSecondsPlayed());
    mvprintw(2, BOARD_OFFSET_X+BOARD_SIZE_X+1, "SCORE:\t %d", this->getScore());
    mvprintw(3, BOARD_OFFSET_X+BOARD_SIZE_X+1, "LEVEL:\t %d", this->getLevel());
}

void Game::lockTetromino() {
    if(this->controlledTetromino->isIntersecting(this->grid)){
        this->controlledTetromino->moveUp();
    }
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

    vector<int> linesCleared;
    for(int y = 0; y < this->controlledTetromino->getSizeY(); y++){
        bool add = true;
        for(int x = 0; x < BOARD_SIZE_X-1; x++){
            if(this->grid[x][tetrY+y] == 0) add = false;
        }
        if(add) linesCleared.push_back(tetrY+y);
    }
    if(!linesCleared.empty()){
        updateScore(linesCleared.size());
        for(int i = 0; i < linesCleared.size(); i++){
            int line = linesCleared[i];
            for(int x = 0; x < BOARD_SIZE_X; x++){
                int temp = -1;
                for(int y = 0; y <= line; y++){
                    if(temp == -1){
                        temp = this->grid[x][y];
                        this->grid[x][y] = 0;
                    }else{
                        int t = temp;
                        temp = this->grid[x][y];
                        this->grid[x][y] = t;
                    }
                }
            }
        }
        for(int x = 0; x < BOARD_SIZE_X; x++){
            this->grid[x][0] = 0;
        }
    }

    delete this->controlledTetromino;
    this->controlledTetromino = this->nextTetromino;
    this->nextTetromino = new Tetromino();
}

void Game::updateScore(int numLines) {
    //Update score(Uses NES Tetris score values)
    switch(numLines){
        case 1:
            this->score+=40*(this->getLevel()+1);
            break;
        case 2:
            this->score+=100*(this->getLevel()+1);
            break;
        case 3:
            this->score+=300*(this->getLevel()+1);
            break;
        case 4:
            this->score+=1200*(this->getLevel()+1);
            break;
    }
    this->linesThisLevel+=numLines;
    if(this->linesThisLevel >= LINES_PER_LEVEL){
        level++;
        this->updateInterval/=1.5;
        this->linesThisLevel%=LINES_PER_LEVEL;
    }
}

void Game::swapHeld() {
    if(this->heldTetromino == NULL){
        this->nextTetromino->move(this->controlledTetromino->getX(), this->controlledTetromino->getY());
        if(nextTetromino->isIntersecting(this->grid)){
            nextTetromino->reset();
            return;
        }
        this->heldTetromino = this->controlledTetromino;
        this->controlledTetromino = this->nextTetromino;
        this->nextTetromino = new Tetromino();
    }else{
        Tetromino* temp = this->controlledTetromino;
        this->heldTetromino->move(this->controlledTetromino->getX(), this->controlledTetromino->getY());
        if(heldTetromino->isIntersecting(this->grid)){
            heldTetromino->reset();
            return;
        }
        this->controlledTetromino = this->heldTetromino;
        this->heldTetromino = temp;
        this->heldTetromino->reset();
    }
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

