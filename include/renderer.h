#ifndef LMGB_RENDERER_H
#define LMGB_RENDERER_H

#include <array>
#include <stdexcept>

#include <defs.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shaders.hpp>

namespace lmgb {

class renderer {
public:
  static constexpr int SCREEN_WIDTH = 160;
  static constexpr int SCREEN_HEIGHT = 144;
  static constexpr int WINDOW_SCALE = 4;

  renderer(const char *game_title, const char *vs_path, const char *fs_path);
  ~renderer();

  renderer(const renderer &) = delete;
  renderer &operator=(const renderer &) = delete;

  void render(const std::uint32_t *framebuffer);
  bool should_close() const;

private:
  GLFWwindow *window_ = nullptr;

  unsigned int vao_ = 0;
  unsigned int vbo_ = 0;
  unsigned int texture_ = 0;

  shaders shader_;

  inline static constexpr std::array<float, 16> vertices_ = {
      // pos        // tex coords
      -1.0f, -1.0f, 0.0f, 1.0f,
       1.0f, -1.0f, 1.0f, 1.0f,
      -1.0f,  1.0f, 0.0f, 0.0f,
       1.0f,  1.0f, 1.0f, 0.0f,
  };
};

} // namespace lmgb

#endif