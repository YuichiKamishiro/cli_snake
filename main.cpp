#include "snake.hpp"
#include <curses.h>
#include <chrono>
#include <iostream>

int main(int argc, char *argv[]){
    int speed = 200;
    
    std::cout << "Enter your speed[px/milli], default - 200 : ";
    std::cin >> speed;

    initscr();
    noecho();
    cbreak();
    
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);

    int y, x, winy, winx;
    getmaxyx(stdscr, y, x);
    
    snake sn({1, 1});
    
    WINDOW *win = newwin(y / 2, y, 1, 1);
    getmaxyx(win, winy, winx);

    curs_set(FALSE);
    nodelay(stdscr, TRUE);
    srand(time(0));
    
    while(sn.terminate == false){
        werase(win);

        sn.collision(0, winy - 1, 0, winx - 1);
        sn.control();
        sn.spawn(1, winy - 2, 1, winx - 2);
        sn.move(win, speed);

        mvwprintw(stdscr, 0, 2, "score : %d", sn.snake_arr.size() - 1);       
        wrefresh(stdscr);
        
        box(win, 0, 0);
        wrefresh(win);
    }
    endwin();
}
