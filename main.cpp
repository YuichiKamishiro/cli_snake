#include "snake.hpp"
#include <chrono>
#include <curses.h>
#include <exception>

int main(){
    initscr();
    
    int y, x;
    getmaxyx(stdscr, y, x);
    snake sn({y / 2, x / 2});
    noecho();
    cbreak();

    std::chrono::steady_clock::time_point st = std::chrono::steady_clock::now();

    while(true){
        std::chrono::steady_clock::time_point en = std::chrono::steady_clock::now();
        if(std::chrono::duration_cast<std::chrono::milliseconds>(en - st).count() >= 100){
            clear();
            st = std::chrono::steady_clock::now();
        }

        sn.control();
        sn.move();
        sn.draw(stdscr);
        
        box(stdscr, 0, 0);
    
        refresh();
    }

    endwin();
}
