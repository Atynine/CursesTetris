//
// Created by rylan on 4/18/19.
//

#include <curses.h>
#include <algorithm>
#include "tetromino.h"

Tetromino::Tetromino() {
    int colors[] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN};
    this->loc_x = BOARD_SIZE_X/2-2;
    this->loc_y = 0;
    this->color = colors[rand()%5];
    int type = rand()%7;
    switch(type){
        case TETROMINO_TYPE::I:
            this->grid.push_back({1,1,1,1});
            break;
        case TETROMINO_TYPE::O:
            this->grid.push_back({1,1});
            this->grid.push_back({1,1});
            break;
        case TETROMINO_TYPE::T:
            this->grid.push_back({1,1,1});
            this->grid.push_back({0,1,0});
            break;
        case TETROMINO_TYPE::J:
            this->grid.push_back({1,1,1});
            this->grid.push_back({0,0,1});
            break;
        case TETROMINO_TYPE::L:
            this->grid.push_back({1,1,1});
            this->grid.push_back({1,0,0});
            break;
        case TETROMINO_TYPE::S:
            this->grid.push_back({0,1,1});
            this->grid.push_back({1,1,0});
            break;
        case TETROMINO_TYPE::Z:
            this->grid.push_back({1,1,0});
            this->grid.push_back({0,1,1});
            break;
    }
}

void Tetromino::render() {
    attron(COLOR_PAIR(this->color));
    for(int y = 0; y < this->grid.size(); y++){
        for(int x = 0; x < this->grid[y].size(); x++){
            if(this->grid[y][x]) mvprintw(this->loc_y + y +BOARD_OFFSET_Y, this->loc_x + x + BOARD_OFFSET_X, " ");
        }
    }
    attroff(COLOR_PAIR(this->color));
}

void Tetromino::rotateLeft(int gameGrid[BOARD_SIZE_X][BOARD_SIZE_Y]) {
    int size = this->getSizeX();
    for(int y = 0; y < this->grid.size(); y++){
        std::reverse(this->grid[y].begin(), this->grid[y].end());
    }
    this->transpose();
    this->loc_x+=size-this->getSizeX();
    if(isIntersecting(gameGrid)) rotateRight(gameGrid);
}

void Tetromino::rotateRight(int gameGrid[BOARD_SIZE_X][BOARD_SIZE_Y]) {
    int size = this->getSizeX();
    this->transpose();
    for(int y = 0; y < this->grid.size(); y++){
        std::reverse(this->grid[y].begin(), this->grid[y].end());
    }

    this->loc_x+=size-this->getSizeX();
    if(isIntersecting(gameGrid)) rotateLeft(gameGrid);
}

void Tetromino::transpose() {
    vector<vector<bool>> newGrid;

    for(int col = 0; col < this->grid[0].size(); col++){
        vector<bool> newRow;
        for(int row = 0; row < this->grid.size(); row++){
            newRow.push_back(this->grid[row][col]);
        }
        newGrid.push_back(newRow);
    }

    this->grid = newGrid;
}

void Tetromino::update() {
    this->loc_y++;
}

void Tetromino::moveUp() {
    this->loc_y--;
}


bool Tetromino::contains(int x, int y) {
    int arrX = x - this->getX();
    int arrY = y - this->getY();
    if(arrX<0 || arrY<0) return false;
    if(this->grid.size() <= arrY) return false;
    if(this->grid[0].size() <= arrX) return false;
    return this->grid[arrY][arrX];
}

void Tetromino::moveLeft(int gameGrid[BOARD_SIZE_X][BOARD_SIZE_Y]) {
    if(this->loc_x-1 < 0) return;
    this->loc_x--;

    if(this->isIntersecting(gameGrid)) this->loc_x++;
}

void Tetromino::moveRight(int gameGrid[BOARD_SIZE_X][BOARD_SIZE_Y]) {
    if(this->loc_x+this->getSizeX()+1 >= BOARD_SIZE_X) return;
    this->loc_x++;

    if(this->isIntersecting(gameGrid)) this->loc_x--;
}

bool Tetromino::isIntersecting(int (*gameGrid)[20]) {
    for(int y = 0; y < this->grid.size(); y++){
        for(int x = 0; x < this->grid[y].size(); x++){
            if(this->grid[y][x] && gameGrid[this->loc_x + x][this->loc_y + y]){
                return true;
            }
        }
    }
    return false;
}

