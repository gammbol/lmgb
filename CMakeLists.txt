cmake_minimum_required(VERSION 3.60)
project(lmgb VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 11)

add_executable(lmgb src/cpu.cpp src/interrupts.cpp src/mem.cpp
        src/input.cpp src/timer.cpp)

target_include_directories(lmgb PUBLIC include)