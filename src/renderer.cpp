#include <renderer.h>

lmgb::renderer::renderer(const char *game_title, const char *vs_path,
                         const char *fs_path)
    : shader_(vs_path, fs_path) {
  if (!glfwInit()) {
    throw std::runtime_error("failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window_ = glfwCreateWindow(SCREEN_WIDTH * WINDOW_SCALE,
                             SCREEN_HEIGHT * WINDOW_SCALE, game_title, nullptr,
                             nullptr);

  if (window_ == nullptr) {
    glfwTerminate();
    throw std::runtime_error("failed to create GLFW window");
  }

  glfwMakeContextCurrent(window_);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    glfwDestroyWindow(window_);
    glfwTerminate();
    throw std::runtime_error("failed to initialize GLAD");
  }

  glfwSwapInterval(1);

  glViewport(0, 0, SCREEN_WIDTH * WINDOW_SCALE, SCREEN_HEIGHT * WINDOW_SCALE);

  shader_.compile_shaders();
  shader_.link_program();
  shader_.use_program();
  shader_.setInt("screenTexture", 0);

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);

  glBindVertexArray(vao_);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float),
               vertices_.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                        reinterpret_cast<void *>(sizeof(float) * 2));
  glEnableVertexAttribArray(1);

  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, SCREEN_WIDTH, SCREEN_HEIGHT, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

lmgb::renderer::~renderer() {
  if (window_ != nullptr) {
    glfwMakeContextCurrent(window_);
  }

  if (texture_ != 0) {
    glDeleteTextures(1, &texture_);
  }

  if (vbo_ != 0) {
    glDeleteBuffers(1, &vbo_);
  }

  if (vao_ != 0) {
    glDeleteVertexArrays(1, &vao_);
  }

  if (window_ != nullptr) {
    glfwDestroyWindow(window_);
  }

  glfwTerminate();
}

void lmgb::renderer::render(const std::uint32_t *framebuffer) {
  glBindTexture(GL_TEXTURE_2D, texture_);

  glTexSubImage2D(
      GL_TEXTURE_2D,
      0,
      0,
      0,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      framebuffer
  );

  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glfwSwapBuffers(window_);
  glfwPollEvents();
}

void lmgb::renderer::poll_events() {
  glfwPollEvents();
}

bool lmgb::renderer::should_close() const {
  return glfwWindowShouldClose(window_);
}