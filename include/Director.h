#pragma once

#include <deque>
#include <random>
#include <curses.h>     // cbreak() noecho() getch()...
#include <csignal>
#include <sys/time.h>  // NOLINT

enum End{
    CrashWall,
    CrashSelf,
    Win
};

class Director {
public:
    static void Init();
    static void Input();

private:
    static void DrawSnake();
    static void GameOver(End state);
    static void SetTicker(int n_msecs);
    static void SigHandler(int signum);
};

