#include <ctime>
#include <curses.h>
#include <chrono>
#include <functional>
#include <vector>

class point {
public:
    int y, x;
    point(int y, int x) : y(y), x(x) {}
    point() : y(0), x(0) {}
};

class node {
public:
    point past_pos, current_pos;
    node(point current_pos) : current_pos(current_pos) {} 
};

class snake {
public:
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point spawn_timer;
    
    std::vector<node> snake_arr;
    bool eaten = true;
    point apple_pos;
    int dir = '0';

    snake(std::pair<int, int>);

    void move();
    void spawn(int, int, int, int);
    void control();
    void collision(int, int, int, int); 
    void draw(WINDOW *);
};

void snake::move() {
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    if(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() >= 200){
        start = std::chrono::steady_clock::now();

        snake_arr[0].past_pos.x = snake_arr[0].current_pos.x;
        snake_arr[0].past_pos.y = snake_arr[0].current_pos.y;
        
        if(dir == 'w') { snake_arr[0].current_pos.y -= 1; }
        if(dir == 's') { snake_arr[0].current_pos.y += 1; }
        if(dir == 'a') { snake_arr[0].current_pos.x -= 1; }
        if(dir == 'd') { snake_arr[0].current_pos.x += 1; }

        for(int i = 1; i < snake_arr.size(); ++i){
            snake_arr[i].past_pos.x = snake_arr[i].current_pos.x;
            snake_arr[i].past_pos.y = snake_arr[i].current_pos.y;

            snake_arr[i].current_pos.x = snake_arr[i - 1].past_pos.x;
            snake_arr[i].current_pos.y = snake_arr[i - 1].past_pos.y;
        }
    }
}

void snake::control() {
    char c = getch();
    
    switch(c) {
        case 'w': dir = 'w'; break;
        case 'a': dir = 'a'; break;
        case 's': dir = 's'; break;
        case 'd': dir = 'd'; break;
    }
}

void snake::draw(WINDOW *win){ 
    for(int i = 0; i < snake_arr.size(); ++i) { mvwaddch(win, snake_arr[i].current_pos.y, snake_arr[i].current_pos.x, '+'); }
    mvwaddch(win, apple_pos.y, apple_pos.x, '*');
}

snake::snake(std::pair<int, int> p) {
    snake_arr.push_back(node({p.first, p.second}));
    start = std::chrono::steady_clock::now();
    spawn_timer = std::chrono::steady_clock::now();
}


void snake::spawn(int miny, int maxy, int minx, int maxx) {
    if(eaten) {
        point rpoint(rand() % maxy + miny, rand() % maxx + minx);
        apple_pos = rpoint;
        eaten = false;
    } else if(!eaten){
        if(snake_arr[0].current_pos.y == apple_pos.y
        && snake_arr[0].current_pos.x == apple_pos.x){ 
            snake_arr.push_back(node(point(snake_arr[snake_arr.size() - 1].past_pos.y,
            snake_arr[snake_arr.size() - 1].past_pos.x)));  
            eaten = true;
            apple_pos.x = 0; apple_pos.y = 0;
        }
    }
}

void snake::collision(int miny, int maxy, int minx, int maxx) {
    if(snake_arr[0].current_pos.x >= maxx || snake_arr[0].current_pos.x <= minx) exit(3);
    if(snake_arr[0].current_pos.y >= maxy || snake_arr[0].current_pos.y <= miny) exit(3);
    for(int i = 1; i < snake_arr.size(); ++i) {
        if(snake_arr[0].current_pos.y == snake_arr[i].current_pos.y 
        && snake_arr[0].current_pos.x == snake_arr[i].current_pos.x) { 
            exit(3);
        }
    }
}
