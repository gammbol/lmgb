#ifndef LMGB_RENDERER_H
#define LMGB_RENDERER_H



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shaders.hpp>

#include <defs.h>

namespace lmgb {

class renderer {
  const int SCR_WIDTH = 160;
  const int SCR_HEIGHT = 144;

  float vertices[16] = {
    // position   // texture
    -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
  };

  GLFWwindow *window;

  // vao, vbo
  unsigned int vao;
  unsigned int vbo;

  // texture buffer
  unsigned int texture;

public:
  renderer(char *game_title, char *vs, char *fs);

  void render(const byte *framebuffer);
};

};

#endif