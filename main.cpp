#include "snake.hpp"
#include <chrono>
#include <curses.h>
#include <exception>

int main(){
    initscr();
    noecho();
    cbreak();
    
    int y, x, winy, winx;
    getmaxyx(stdscr, y, x);
    
    snake sn({5, 5});
    
    WINDOW *win = newwin(y / 2 + 5, x / 2, y / 4, x / 4);
    getmaxyx(win, winy, winx);

    std::chrono::steady_clock::time_point st = std::chrono::steady_clock::now();

    curs_set(0);

    while(true){
        std::chrono::steady_clock::time_point en = std::chrono::steady_clock::now();
        if(std::chrono::duration_cast<std::chrono::milliseconds>(en - st).count() >= 100){
            wclear(win);
            st = std::chrono::steady_clock::now();
        }


        sn.collision(1, winy - 1, 1, winx - 1);
        sn.control();
        sn.spawn(1, winy - 2, 1, winx - 2);
        sn.move();

        box(win, 0, 0);
        sn.draw(win);
    
        refresh();
        wrefresh(win);
    }

    endwin();
}
