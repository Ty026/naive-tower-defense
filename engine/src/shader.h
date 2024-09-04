#pragma once
#include "gl.h"
#include <string_view>

class Shader {
public:
  Shader();
  ~Shader();

  bool CompileFromFile(std::string_view vert_filename, std::string_view frag_filename);
  bool CompileFromSource(std::string_view vert_source, std::string_view frag_source);
  void Bind();

private:
  GLuint Compile(std::string_view source, GLenum type);
  GLuint program_;
};
