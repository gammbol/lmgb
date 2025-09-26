cmake_minimum_required(VERSION 3.60)
project(lmgb VERSION 0.0.0)

add_executable(lmgb src/cpu.cpp src/interrupts.cpp src/mbc.cpp src/mem.cpp
        src/input.cpp)

target_include_directories(lmgb PUBLIC include)