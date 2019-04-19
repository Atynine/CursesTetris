//
// Created by rylan on 4/18/19.
//

#ifndef TETRIS_TETROMINO_H
#define TETRIS_TETROMINO_H

#include "vector"

#define BOARD_SIZE_X 11
#define BOARD_SIZE_Y 20
#define BOARD_OFFSET_X 36
#define BOARD_OFFSET_Y 1

using namespace std;

enum TETROMINO_TYPE{
    I, O, T, J, L, S, Z
};

class Tetromino {
public:
    Tetromino();
    void render(int xOffset, int yOffset);
    void rotateLeft(int gameGrid[BOARD_SIZE_X][BOARD_SIZE_Y]);
    void rotateRight(int gameGrid[BOARD_SIZE_X][BOARD_SIZE_Y]);
    void moveLeft(int gameGrid[BOARD_SIZE_X][BOARD_SIZE_Y]);
    void moveRight(int gameGrid[BOARD_SIZE_X][BOARD_SIZE_Y]);
    void update();
    void moveUp();
    void move(int x, int y);

    int getX(){ return this->loc_x; }
    int getY(){ return this->loc_y; }
    int getSizeX(){ return this->grid[0].size(); }
    int getSizeY(){ return this->grid.size(); }
    int getColor(){ return this->color; }
    bool contains(int x, int y);
    bool isIntersecting(int gameGrid[BOARD_SIZE_X][BOARD_SIZE_Y]);
    void reset();
private:
    int loc_x, loc_y, color;
    vector<vector<bool>> grid;
    void transpose();
};


#endif //TETRIS_TETROMINO_H
