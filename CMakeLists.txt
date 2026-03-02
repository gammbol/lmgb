cmake_minimum_required(VERSION 3.60)
project(lmgb VERSION 0.0.0)

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)

file(COPY shaders DESTINATION .)

add_executable(lmgb 
  src/cpu.cpp 
  src/input.cpp 
  src/interrupts.cpp 
  src/lmgb.cpp 
  src/mem.cpp 
  src/timer.cpp 
  src/graphics.cpp
  src/shaders.cpp
  src/glad.c
  src/renderer.cpp
)

target_include_directories(lmgb PUBLIC include)

target_link_libraries(lmgb PRIVATE glfw glm)