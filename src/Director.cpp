#include "../include/Director.h"

// Snake components.
struct Node{
    Node(int x, int y) : pos_x_(x), pos_y_(y) {}

    int pos_x_;
    int pos_y_;
};
std::deque<Node> snake; // NOLINT

// Mark the direction.
enum Direction{
    Up,
    Down,
    Left,
    Right
};
Direction current_direction(Right);

struct Direct{
    Direct(int x, int y) : x_(x), y_(y) {}

    int x_;
    int y_;
};
Direct direct(1,0); // NOLINT

// Food position
int food_x;
int food_y;

// Score.
int scores;

void Director::Init()
{
    // Init environment
    initscr();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    refresh();

    // Draw the wall
    move(1, 0);
    for (int i = 0; i < COLS; ++i) { printw("-"); }
    move(0, COLS/2 - 4);
    printw("scores: 0");

    // Init food position
    food_x = 6;
    food_y = 8;

    // Init snake and it's position
    snake.emplace_back(3,6);

    // Set Timer
    SetTicker(80);
    signal(SIGALRM, Director::SigHandler);
}

void Director::Input()
{
    while (true)
    {
        int ch = getch();
        switch (ch)
        {
            case KEY_LEFT:
                if (current_direction != Right) { current_direction = Left; direct = Direct(-1,0); }
                break;
            case KEY_RIGHT:
                if (current_direction != Left) { current_direction = Right; direct = Direct(1,0); }
                break;
            case KEY_UP:
                if (current_direction != Down) { current_direction = Up; direct = Direct(0,-1); }
                break;
            case KEY_DOWN:
                if (current_direction != Up) { current_direction = Down; direct = Direct(0,1); }
                break;
            default:
                break;
        }
        if (ch == 't')
            break;
    }
}

void Director::DrawSnake()
{
    move(food_y, food_x);
    printw("$");

    bool eaten = false;

    // Change the direction.
    snake.emplace_front(snake.at(0).pos_x_ + direct.x_, snake.at(0).pos_y_ + direct.y_);

    // Determine if hit the wall.
    if (snake.at(0).pos_x_ <=0 || snake.at(0).pos_x_ >= COLS-1 ||
        snake.at(0).pos_y_ <= 1 || snake.at(0).pos_y_ >= LINES-1)
    { GameOver(CrashWall); return; }

    // Determine if hit itself.
    if ('O' == mvinch(snake.at(0).pos_y_, snake.at(0).pos_x_))
    { GameOver(CrashSelf); return; }

    if (snake.at(0).pos_x_ == food_x && snake.at(0).pos_y_ == food_y)
    {
        eaten = true;
        // Update the score and determine if win.
        scores += 10;
        if (scores >= 60) { GameOver(Win); return; }
        move(0, COLS/2 + 4);
        printw("%d", scores);
        // Generate another food.
        food_x = (int)random() % (COLS - 5);
        food_y = (int)random() % (LINES - 5);
    }

    if (!eaten)
    {
        move(snake.back().pos_y_, snake.back().pos_x_);
        printw(" ");
        snake.pop_back();
    }

    move(snake.front().pos_y_, snake.front().pos_x_);
    printw("O");
}

void Director::GameOver(End state)
{
    clear();
    move( LINES / 2 - 1, COLS / 2 - 12 );
    switch (state)
    {
        case CrashWall:
            addstr("Crash the wall. Game over!");
            break;
        case CrashSelf:
            addstr("Crash yourself. Game over!");
            break;
        case Win:
            addstr("Congratulations on your victory, warrior!");
            break;
    }
    refresh();
    // CLose the timer.
    SetTicker(0);
}

void Director::SetTicker(int n_msecs)
{
    struct itimerval it{};
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = 1000 * n_msecs;

    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 1000 * n_msecs;

    setitimer(ITIMER_REAL, &it, nullptr);
}

void Director::SigHandler(int signum)
{
    signal(SIGALRM, SigHandler);
    DrawSnake();
    refresh();
}
