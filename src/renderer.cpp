#include <renderer.h>

// TODO: destructor
lmgb::renderer::renderer(lmgb::mem *memory, char *game_title, char *vs,
                         char *fs)
    : memory(memory) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // making the window unresizable
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, game_title, nullptr, nullptr);

  if (window == nullptr) {
    glfwTerminate();
    throw new std::runtime_error("failed to create a window");
  }

  glfwMakeContextCurrent(window);

  // init glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    glfwTerminate();
    throw new std::runtime_error("failed to init glad");
  }

  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  // generating buffers
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenTextures(1, &texture);

  // binding
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // transfering data
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(float), vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                        (void *)(sizeof(float) * 2));
  glEnableVertexAttribArray(1);

  glTexImage2D(GL_TEXTURE_2D, 0,
               GL_RGBA8, // как хранится в VRAM
               SCR_WIDTH, SCR_HEIGHT, 0,
               GL_RGBA,          // формат данных
               GL_UNSIGNED_BYTE, // тип данных
               nullptr);
}

lmgb::renderer::~renderer() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteTextures(1, &texture);
  glfwTerminate();
}

void lmgb::renderer::render(const byte *framebuffer) {
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGBA,
                  GL_UNSIGNED_BYTE, framebuffer);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
