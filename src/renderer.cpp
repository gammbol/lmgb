#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <shaders.hpp>

const int SCR_WIDTH = 160;
const int SCR_HEIGHT = 144;

const float POINT_SCALE = 1.0f;

std::vector<float> genPoints();
std::vector<float> generatePixels160x144();

// i'll leave it like this for now
// later i'm gonna make it right
int renderer(int argc, char *argv[]) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "test", nullptr, nullptr);

  if (window == nullptr) {
    std::cout << "failed to create a window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "failed to init glad" << std::endl;
    return -1;
  }

  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  // POINTS
  // ----------------------------
  std::vector<float> pixels{generatePixels160x144()};

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, pixels.size() * sizeof(float), pixels.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5,
                        (void *)(sizeof(float) * 2));
  glEnableVertexAttribArray(1);

  shaders shd{"shaders/vertex.vs", "shaders/fragment.fs"};
  shd.compile_shaders();
  shd.link_program();
  shd.use_program();

  shd.setVec2("uResolution", (float)SCR_WIDTH, (float)SCR_HEIGHT);

  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents();

    shd.use_program();

    // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_POINTS, 0, 160 * 144);
    // glBindVertexArray(0);
  }

  glfwTerminate();
  return 0;
}

float palette[4] = {
    1.0f, // 0
    0.7f, // 1
    0.4f, // 2
    0.1f  // 3
};

uint8_t tile[8][8] = {{1, 1, 1, 1, 1, 1, 1, 1}, {2, 3, 3, 3, 3, 3, 3, 2},
                      {2, 1, 0, 0, 0, 0, 1, 0}, {2, 1, 0, 0, 1, 0, 0, 1},
                      {2, 1, 0, 0, 1, 0, 1, 1}, {2, 1, 0, 1, 1, 0, 1, 1},
                      {3, 1, 0, 0, 1, 0, 0, 1}, {2, 3, 3, 3, 3, 3, 3, 2}};

std::vector<float> genPoints() {
  std::vector<float> pixels;
  const int scale = 5; // 8 * 5 = 40
  const int tileSize = 8;

  for (int ty = 0; ty < tileSize; ty++) {
    for (int tx = 0; tx < tileSize; tx++) {
      float c = palette[tile[ty][tx]];

      // каждый пиксель → блок scale×scale точек
      for (int sy = 0; sy < scale; sy++) {
        for (int sx = 0; sx < scale; sx++) {
          float x = tx * scale + sx;
          float y = ty * scale + sy;

          pixels.push_back(x);
          pixels.push_back(y);
          pixels.push_back(c);
          pixels.push_back(c);
          pixels.push_back(c);
        }
      }
    }
  }

  return pixels;
}

std::vector<float> generatePixels160x144() {
  const int W = 160;
  const int H = 144;

  std::vector<float> pixels;
  pixels.reserve(W * H * 5);

  // центр и радиусы
  const float cx = W / 2.0f;
  const float cy = H / 2.0f;
  const float R = 50.0f;
  const float rButton = 8.0f;

  for (int y = 0; y < H; y++) {
    for (int x = 0; x < W; x++) {
      float dx = x - cx;
      float dy = y - cy;
      float dist = std::sqrt(dx * dx + dy * dy);

      float r = 0.85f;
      float g = 0.85f;
      float b = 0.85f; // фон

      if (dist <= R) {
        // чёрный контур
        if (dist > R - 2.0f) {
          r = g = b = 0.0f;
        }
        // кнопка
        else if (std::sqrt(dx * dx + dy * dy) < rButton) {
          r = g = b = 0.9f;
        }
        // горизонтальная линия
        else if (std::abs(dy) < 2.0f) {
          r = g = b = 0.0f;
        }
        // верхняя половина
        else if (dy < 0) {
          r = 0.9f;
          g = 0.1f;
          b = 0.1f;
        }
        // нижняя половина
        else {
          r = g = b = 0.95f;
        }
      }

      pixels.push_back((float)x);
      pixels.push_back((float)y);
      pixels.push_back(r);
      pixels.push_back(g);
      pixels.push_back(b);
    }
  }

  return pixels;
}
