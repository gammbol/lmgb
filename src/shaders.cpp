#include <shaders.hpp>

shaders::shaders(const char *vs_path, const char *fs_path) {
  state = NOS;

  std::ifstream vfs;
  std::ifstream ffs;

  vfs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  ffs.exceptions (std::ifstream::failbit | std::ifstream::badbit);

  try {
    vfs.open(vs_path);
    ffs.open(fs_path);

    std::stringstream vsStream, fsStream;

    vsStream << vfs.rdbuf();
    fsStream << ffs.rdbuf();

    vfs.close();
    ffs.close();

    vss = vsStream.str();
    fss = fsStream.str();

  } catch (std::istream::failure e) {
    std::cerr << "Couldn't process the shaders!" << std::endl;
    std::cerr << e.what() << std::endl;
    return;
  }

  state = LOADED;
}

void shaders::compile_shaders() {
  if (state != LOADED) {
    std::cerr << "Shaders are not loaded! Compilation is not possible!" << std::endl;
    return;
  }

  int success;
  char infoLog[512];

  vs = glCreateShader(GL_VERTEX_SHADER);
  fs = glCreateShader(GL_FRAGMENT_SHADER);

  const char *cvss = vss.c_str();
  const char *cfss = fss.c_str();
  
  glShaderSource(vs, 1, &cvss, NULL);
  glShaderSource(fs, 1, &cfss, NULL);

  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vs, 512, NULL, infoLog);
    std::cerr << "========= Vertex Shader Error =========\n" << infoLog << std::endl;
    return;
  }

  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fs, 512, NULL, infoLog);
    std::cerr << "========= Fragment Shader Error =========\n" << infoLog << std::endl;
    return;
  }

  state = COMPILED;
}

void shaders::link_program() {
  if (state != COMPILED) {
    std::cerr << "Shaders are not compiled! Linking is not possible!" << std::endl;
    return;
  }

  int success;
  char infoLog[512];

  sp = glCreateProgram();

  glAttachShader(sp, vs);
  glAttachShader(sp, fs);
  glLinkProgram(sp);

  glGetProgramiv(sp, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(sp, 512, NULL, infoLog);
    std::cerr << "========= Shader Program Error =========\n" << infoLog << std::endl;
    return;
  }

  glUseProgram(sp);

  glDeleteShader(vs);
  glDeleteShader(fs);

  state = LINKED;
}

void shaders::use_program() {
  if (state != LINKED) {
    std::cerr << "Shaders are not linked! Using is not possible!" << std::endl;
    return;
  }

  glUseProgram(sp);
}

void shaders::setBool(const std::string& name, const bool& value) {
  glUniform1i(glGetUniformLocation(sp, name.c_str()), (int)value);
}

void shaders::setInt(const std::string& name, const int& value) {
  glUniform1i(glGetUniformLocation(sp, name.c_str()), value);
}

void shaders::setFloat(const std::string& name, const float& value) {
  glUniform1f(glGetUniformLocation(sp, name.c_str()), value);
}

void shaders::setMat4(const std::string& name, glm::mat4& value) {
  glUniformMatrix4fv(glGetUniformLocation(sp, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void shaders::setVec2(const std::string& name, const float& fst, const float& snd) {
  glUniform2f(glGetUniformLocation(sp, name.c_str()), fst, snd);
}