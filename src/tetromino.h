//
// Created by rylan on 4/18/19.
//

#ifndef TETRIS_TETROMINO_H
#define TETRIS_TETROMINO_H

#include "vector"

using namespace std;

enum TETROMINO_TYPE{
    I, O, T, J, L, S, Z
};

class Tetromino {
public:
    Tetromino();
    void render();
    void rotateLeft();
    void rotateRight();
    void moveLeft();
    void moveRight();
    void update();


    int getX(){ return this->loc_x; }
    int getY(){ return this->loc_y; }
    int getSizeX(){ return this->grid[0].size(); }
    int getSizeY(){ return this->grid.size(); }
    int getColor(){ return this->color; }
    bool contains(int x, int y);
private:
    int loc_x, loc_y, color;
    vector<vector<bool>> grid;
    void transpose();
};


#endif //TETRIS_TETROMINO_H
