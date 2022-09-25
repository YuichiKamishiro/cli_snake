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
    std::chrono::steady_clock::time_point move_clock;
    
    std::vector<node> snake_arr;
    
    std::string choices[3]{"restart - q", "quit - a", "select - z"};
    
    int choice = 0;
    
    bool eaten = true; bool terminate = false; bool in_menu = false;
    
    point apple_pos;
    
    int dir = '0';

    snake(std::pair<int, int>);

    void start_menu();
    void menu(WINDOW *);
    void restart();
    void move(WINDOW *, int);
    void spawn(int, int, int, int);
    void control();
    void collision(int, int, int, int); 
    void draw(WINDOW *);
};

void snake::move(WINDOW *win, int speed) {

    if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - move_clock).count() >= speed){
        move_clock = std::chrono::steady_clock::now();

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
        wrefresh(win);
    }
}

void snake::control() {
    char c = getch();
    
    switch(c) {
        case 'w': if(dir != 's') dir = 'w'; break;
        case 'a': if(dir != 'd') dir = 'a'; break;
        case 's': if(dir != 'w') dir = 's'; break;
        case 'd': if(dir != 'a') dir = 'd'; break;
    }
}

void snake::draw(WINDOW *win){ 
    for(int i = 1; i < snake_arr.size(); ++i) { mvwaddch(win, snake_arr[i].current_pos.y, snake_arr[i].current_pos.x, '+'); }
    mvwaddch(win, snake_arr[0].current_pos.y, snake_arr[0].current_pos.x, '@');
    mvwaddch(win, apple_pos.y, apple_pos.x, '*');
}

snake::snake(std::pair<int, int> p) {
    snake_arr.push_back(node({p.first, p.second}));
    move_clock = std::chrono::steady_clock::now();
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

void snake::restart() {
    snake_arr.clear();
    snake_arr.push_back(node(point(5, 5)));
    eaten = true;
    dir = '0';
    choice = 0;
    move_clock = std::chrono::steady_clock::now();
}

void snake::start_menu() {
    in_menu = true;
}

void snake::menu(WINDOW *window_to_clear) {
    if(in_menu){
        werase(window_to_clear);
        WINDOW *menu = newwin(5, 20, 0, 0);
        box(menu, 0, 0);
        nodelay(menu, TRUE);

        while(in_menu){
            for(int i = 0; i < 3; ++i) {
                if(choice == i) {
                    wattron(menu, A_REVERSE);
                    mvwprintw(menu, i + 1, 1, choices[i].c_str());
                    wattroff(menu, A_REVERSE);
                    wrefresh(menu);
                } else {
                    mvwprintw(menu, i + 1, 1, choices[i].c_str());
                    wrefresh(menu);     
                }
            }
            char c = wgetch(menu);
            switch(c){
                case 'q': --choice; if(choice < 0) choice = 0; break;
                case 'a': ++choice; if(choice > 1) choice = 1; break;
                case 'z': if(choice == 1) {in_menu = false; terminate = true;} else {in_menu = false; wclear(menu); wrefresh(menu); delwin(menu); restart();} break;
            }
        }
    }
}

void snake::collision(int miny, int maxy, int minx, int maxx) {
    if(snake_arr[0].current_pos.x >= maxx || snake_arr[0].current_pos.x <= minx) start_menu();
    if(snake_arr[0].current_pos.y >= maxy || snake_arr[0].current_pos.y <= miny) start_menu();
    for(int i = 1; i < snake_arr.size(); ++i) {
        if(snake_arr[0].current_pos.y == snake_arr[i].current_pos.y 
        && snake_arr[0].current_pos.x == snake_arr[i].current_pos.x) {
            start_menu();
        }
    }
}
