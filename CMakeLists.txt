cmake_minimum_required(VERSION 3.30)
project(lmgb VERSION 0.0.0)

#add_compile_definitions(LMGB_DEBUG)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)
add_subdirectory(deps/lmshader)

set(glfw_DIR ${CMAKE_BUILD_DIR}/deps/glfw)

file(COPY shaders DESTINATION .)

add_executable(lmgb 
  src/cpu.cpp 
  src/lmgb.cpp 
  src/mem.cpp 
  src/graphics.cpp
  src/glad.c
  src/renderer.cpp
  src/interrupts.cpp
  src/timer.cpp
)

target_include_directories(lmgb PUBLIC include)

target_link_libraries(lmgb PUBLIC glfw glm lmshader)
