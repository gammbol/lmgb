cmake_minimum_required(VERSION 3.60)
project(lmgb VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)

add_executable(lmgb src/cpu.cpp src/input.cpp src/interrupts.cpp src/lmgb.cpp src/mem.cpp src/timer.cpp src/graphics.cpp)

target_include_directories(lmgb PUBLIC include)

target_link_libraries(lmgb PRIVATE glfw glm)