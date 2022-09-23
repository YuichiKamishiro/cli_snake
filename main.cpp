#include "snake.hpp"
#include <curses.h>
#include <chrono>
#include <iostream>

int main(int argc, char *argv[]){
    int speed = 200;
    
    std::cout << "Enter your speed, 200 is default:";
    std::cin >> speed;

    initscr();
    noecho();
    cbreak();
    
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);

    int y, x, winy, winx;
    getmaxyx(stdscr, y, x);
    
    snake sn({5, 5});
    
    WINDOW *win = newwin(y / 2 + 5, x / 2, y / 4, x / 4);
    getmaxyx(win, winy, winx);

    std::chrono::steady_clock::time_point st = std::chrono::steady_clock::now();

    curs_set(FALSE);
    nodelay(stdscr, TRUE);
    srand(time(0));

    while(sn.terminate == false){
        std::chrono::steady_clock::time_point en = std::chrono::steady_clock::now();
        if(std::chrono::duration_cast<std::chrono::milliseconds>(en - st).count() >= 50){
            wclear(win);
            st = std::chrono::steady_clock::now();
        }


        sn.collision(0, winy - 1, 0, winx - 1);
        sn.control();
        sn.spawn(1, winy - 2, 1, winx - 2);
        sn.move(speed);

        mvwprintw(stdscr, y / 4 - 2, x / 4, "score : %d", sn.snake_arr.size() - 1);       

        wattron(win, COLOR_PAIR(2));   
        box(win, 0, 0);
        wattroff(win, COLOR_PAIR(2)); 

        wattron(win, COLOR_PAIR(1));   
        sn.draw(win);
        wattroff(win, COLOR_PAIR(1)); 
       
        wrefresh(win);
    }

    endwin();
}
