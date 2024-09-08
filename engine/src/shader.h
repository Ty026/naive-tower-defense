#pragma once
#include "common.h"
#include "gl.h"
#include <string_view>

class Shader {
public:
  Shader(int max_texture_units);
  ~Shader();

  bool CompileFromFile(std::string_view vert_filename, std::string_view frag_filename);
  bool CompileFromSource(std::string_view vert_source, std::string_view frag_source);
  void Bind();
  void UploadMatrix2x3(std::string_view name, const real *values);
  void UploadIntArray(const std::string_view name, const int *values, int count);

private:
  GLuint      Compile(std::string_view source, GLenum type);
  std::string GenerateFSCodeFromSource(std::string_view template_code);
  GLuint      program_;
  int         max_texture_units_;
};
