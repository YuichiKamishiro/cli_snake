#include <curses.h>
#include <chrono>
#include <functional>
#include <vector>

class node{
public:
    int past_x, past_y, current_x, current_y;
    node(std::pair<int, int>); 
};

class snake{
public:
    std::chrono::steady_clock::time_point start;
    std::vector<node> snake_arr;
    int dir = 'w';

    snake(std::pair<int, int>);
    void move();
    void control();
    void collision(); 
    void draw(WINDOW *);
};

void snake::move(){
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() >= 150){
        start = std::chrono::steady_clock::now();
        for(int i = 0; i < snake_arr.size(); ++i){
        switch(dir){
            case 'w': snake_arr[i].past_y = snake_arr[i].current_y; snake_arr[i].current_y -=1;break;
            case 's': snake_arr[i].past_y = snake_arr[i].current_y; snake_arr[i].current_y +=1;break;
            case 'a': snake_arr[i].past_x = snake_arr[i].current_x; snake_arr[i].current_x -=1;break;
            case 'd': snake_arr[i].past_x = snake_arr[i].current_x; snake_arr[i].current_x +=1;break;
        } 
    }}
}

void snake::control(){
    timeout(1);
    char c = getch();
    switch(c){
        case 'w': dir = 'w';break;
        case 'a': dir = 'a';break;
        case 's': dir = 's';break;
        case 'd': dir = 'd';break;
    }
}

void snake::draw(WINDOW *win){
    for(int i = 0; i < snake_arr.size(); ++i){ 
        mvwprintw(win, snake_arr[i].current_y, snake_arr[i].current_x, "#");
    }
}

node::node(std::pair<int, int> p) : current_y(p.first), current_x(p.second){}

snake::snake(std::pair<int, int> p){
    snake_arr.push_back(node({p.first, p.second}));
    start = std::chrono::steady_clock::now();
}
