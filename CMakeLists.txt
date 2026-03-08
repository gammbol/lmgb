cmake_minimum_required(VERSION 3.60)
project(lmgb VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)
add_subdirectory(deps/lmshader)

set(glfw_DIR ${CMAKE_BUILD_DIR}/deps/glfw)

file(COPY shaders DESTINATION .)

add_executable(lmgb 
  src/cpu.cpp 
  src/input.cpp 
  src/interrupts.cpp 
  src/lmgb.cpp 
  src/mem.cpp 
  src/timer.cpp 
  src/graphics.cpp
  src/glad.c
  src/renderer.cpp
)

target_include_directories(lmgb PUBLIC include)

target_link_libraries(lmgb PUBLIC glfw glm lmshader)
