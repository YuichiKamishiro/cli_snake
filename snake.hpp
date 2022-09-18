#include <curses.h>
#include <chrono>
#include <functional>
#include <vector>

class node{
public:
    short int past_x, past_y, current_x, current_y;
    node(std::pair<short int, short int>); 
};

class snake{
    std::chrono::steady_clock::time_point start;
    std::vector<node> snake_arr;
    
    snake(std::pair<short int, short int>);
    void move();
    void control();
    void collision(); 
    void draw(WINDOW *window);
};
