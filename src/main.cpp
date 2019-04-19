#include <iostream>
#include <curses.h>
#include <zconf.h>
#include <chrono>
#include "game.h"

using namespace std::chrono;

void initScreen(){
    srand(time(NULL));
    initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    cbreak();
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
    nodelay(stdscr, TRUE);
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
    clear();
    nodelay(stdscr, FALSE);

    mvprintw(0, 0, "Game Over. Press 'Q' to quit or 'N' to start again.");
    mvprintw(1, 0, "Time:\t%d\n", game.getSecondsPlayed());
    mvprintw(2, 0, "Score:\t%d\n", game.getScore());
    mvprintw(3, 0, "Level:\t%d\n", game.getLevel());

    int key;
    while(true){
        key = getch();
        if(key == 'q'){
            return false;
        }else if(key == 'n'){
            return true;
        }
    }
}


int main() {
    initScreen();
    while(gameLoop());
    endwin();
}