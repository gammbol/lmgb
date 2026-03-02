#ifndef SHADERS_2BPP_H
#define SHADERS_2BPP_H

#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

enum SHADERS_STATES {
  NOS,
  LOADED,
  COMPILED,
  LINKED
};

class shaders {
  std::string vss, fss;

  unsigned int vs, fs, sp;

  SHADERS_STATES state;

public:
  shaders(const char *vs_path, const char *fs_path);

  void compile_shaders();
  void link_program();
  void use_program();

  void setBool(const std::string& name, const bool& value);
  void setFloat(const std::string& name, const float& value);
  void setInt(const std::string& name, const int& value);
  void setMat4(const std::string& name, glm::mat4& value);
  void setVec2(const std::string& name, const float& fst, const float& snd);
};

#endif