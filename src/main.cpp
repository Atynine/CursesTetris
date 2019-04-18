#include <iostream>
#include <curses.h>
#include <zconf.h>
#include <chrono>
#include "game.h"

#define UPDATE_INTERVAL 150000

using namespace std::chrono;

void initScreen(){
    srand(time(NULL));
    initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    cbreak();
    nodelay(stdscr, TRUE);
    start_color();
    init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(COLOR_RED, COLOR_BLACK, COLOR_RED);
    init_pair(COLOR_GREEN, COLOR_BLACK, COLOR_GREEN);
    init_pair(COLOR_YELLOW, COLOR_BLACK, COLOR_YELLOW);
    init_pair(COLOR_BLUE, COLOR_BLACK, COLOR_BLUE);
    init_pair(COLOR_MAGENTA, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(COLOR_CYAN, COLOR_BLACK, COLOR_CYAN);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
}

/**
 * The core loop of the game
 * @return false if the player is finished; true otherwise
 */
bool gameLoop(){
    Game game;
    high_resolution_clock::time_point lastUpdate = high_resolution_clock::now();
    while(!game.hasQuit() && !game.hasEnded()){

        high_resolution_clock::time_point up = high_resolution_clock::now();
        game.update((up - lastUpdate).count());
        lastUpdate = up;
        game.render();
        usleep(35000);
    }
    if(game.hasQuit()) return false;
    return true;
}


int main() {
    initScreen();
    while(gameLoop());
    endwin();
}