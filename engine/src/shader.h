#pragma once
#include "common.h"
#include "gl.h"
#include <string_view>

class Shader {
public:
  Shader();
  ~Shader();

  bool CompileFromFile(std::string_view vert_filename, std::string_view frag_filename);
  bool CompileFromSource(std::string_view vert_source, std::string_view frag_source);
  void Bind();
  void UploadMatrix2x3(std::string_view name, const real *values);

private:
  GLuint Compile(std::string_view source, GLenum type);
  GLuint program_;
};
