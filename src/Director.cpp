#include "../include/Director.h"

// snake is composed of Node.
struct Node{
    Node(int x, int y) : pos_x_(x), pos_y_(y) {}

    int pos_x_;
    int pos_y_;
};
std::deque<Node> snake; // NOLINT

struct Direction{
    Direction(int x, int y) : x_(x), y_(y) {}

    bool operator== (const Direction& direct) const { return this->x_ == direct.x_ && this->y_ == direct.y_; }

    int x_;
    int y_;
};
Direction current_direction(1, 0); // NOLINT

// Guide snake in its course.
std::deque<Direction> direction_buffer; // NOLINT

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
    snake.emplace_back(3, 6);

    // Set Timer
    SetTicker(300);
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
                if ( !(current_direction == Direction(1, 0)) )    // current_direction != Right
                {
                    current_direction = Direction(-1, 0);
                    direction_buffer.push_back(current_direction);
                }
                break;
            case KEY_RIGHT:
                if ( !(current_direction == Direction(-1, 0)) )
                {
                    current_direction = Direction(1, 0);
                    direction_buffer.push_back(current_direction);
                }
                break;
            case KEY_UP:
                if ( !(current_direction == Direction(0, 1)) )
                {
                    current_direction = Direction(0, -1);
                    direction_buffer.push_back(current_direction);
                }
                break;
            case KEY_DOWN:
                if ( !(current_direction == Direction(0, -1)) )
                {
                    current_direction = Direction(0, 1);
                    direction_buffer.push_back(current_direction);
                }
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

    // Fetches direction from direction_buffer.
    if (direction_buffer.empty())
    {
        snake.emplace_front(snake.at(0).pos_x_ + current_direction.x_, snake.at(0).pos_y_ + current_direction.y_);
    }
    else
    {
        snake.emplace_front(snake.at(0).pos_x_ + direction_buffer.front().x_, snake.at(0).pos_y_ + direction_buffer.front().y_);
        direction_buffer.pop_front();
    }

    // Determine if hit the wall.
    if (snake.at(0).pos_x_ <= 0 || snake.at(0).pos_x_ >= COLS - 1 ||
        snake.at(0).pos_y_ <= 1 || snake.at(0).pos_y_ >= LINES - 1)
    { GameOver(CrashWall); return; }

    // Determine if hit itself.
    if ('O' == mvinch(snake.at(0).pos_y_, snake.at(0).pos_x_))
    { GameOver(CrashSelf); return; }

    // Snake get the food.
    // No need to remove the last node at this time.
    // So eaten = true;
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

    // Snake did not get food in this movement.
    // Remove the last node of the snake and
    // use " " to erase the last node which was originally displayed on the screen (O).
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
