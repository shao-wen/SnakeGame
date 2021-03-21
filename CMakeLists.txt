cmake_minimum_required(VERSION 3.16)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 11)

link_libraries(curses)

add_executable(SnakeGame src/main.cpp src/Director.cpp)