#include <curses.h>
#include <chrono>
#include <functional>
#include <vector>

class node{
public:
    int past_x, past_y, current_x, current_y;
    node(std::pair<short int, short int>); 
};

class snake{
public:
    std::chrono::steady_clock::time_point start;
    std::vector<node> snake_arr;
    int dir = 0;
    
    snake(std::pair<short int, short int>);
    void move();
    void control();
    void collision(); 
    void draw(WINDOW *);
};

void snake::move(){
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() >= 300){
        for(int i = 0; i < snake_arr.size(); ++i){
        start = std::chrono::steady_clock::now();
        switch(dir){
            case 'w': snake_arr[i].past_y = snake_arr[i].current_y;snake_arr[i].current_y -=1;break;
            case 'a': snake_arr[i].past_y = snake_arr[i].current_y;snake_arr[i].current_y +=1;break;
            case 's': snake_arr[i].past_x = snake_arr[i].current_x;snake_arr[i].current_x -=1;break;
            case 'd': snake_arr[i].past_x = snake_arr[i].current_x;snake_arr[i].current_x +=1;break;
        } 
    }}
}

void snake::control(){
    if(getch() == 'w')      dir = 'w';
    else if(getch() == 'a') dir = 'a';
    else if(getch() == 's') dir = 's';
    else if(getch() == 'd') dir = 'd';
}

void snake::draw(WINDOW *win){
    for(int i = 0; i < snake_arr.size(); ++i) 
        mvwprintw(win, snake_arr[i].current_y, snake_arr[i].current_x, "@");
}
