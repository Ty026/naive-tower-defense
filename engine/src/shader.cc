#include "shader.h"
#include "log.h"
#include "file-reader.h"
#include <cassert>

namespace {
std::string LoadFile(std::string_view path) {
  FileReader  file_reader;
  std::string content;
  if (!file_reader.Open(path)) {
    Log::Error("Unable to load file: {}", path);
    return content;
  }
  content.resize(file_reader.size() + 1);
  file_reader.Read(&content[0], file_reader.size());
  content[content.size()] = '\0';
  return content;
}
} // namespace

Shader::Shader() {
  GL_CHECK(program_ = glCreateProgram());
}

Shader::~Shader() {
  GL_CHECK(glDeleteProgram(program_));
}

GLuint Shader::Compile(const std::string_view source, GLenum type) {
  GLuint shader;
  GL_CHECK(shader = glCreateShader(type));
  const char *str = source.data();
  GL_CHECK(glShaderSource(shader, 1, &str, nullptr));
  GL_CHECK(glCompileShader(shader));
  GLint success{0};
  GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
  if (!success) {
    GLint length{0};
    GL_CHECK(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
    std::string log(length, ' ');
    GL_CHECK(glGetShaderInfoLog(shader, length, &length, &log[0]));
    Log::Error("Failed to compile shader: {}.", log);
    Log::Error("{}", source);
    GL_CHECK(glDeleteShader(shader));
    return GL_INVALID_VALUE;
  }
  return shader;
}

void Shader::Bind() {
  glUseProgram(program_);
}

bool Shader::CompileFromFile(std::string_view vert_filename, std::string_view frag_filename) {
  auto vert_source = LoadFile(vert_filename);
  auto frag_source = LoadFile(frag_filename);
  return CompileFromSource(vert_source, frag_source);
}

bool Shader::CompileFromSource(std::string_view vert_source, std::string_view frag_source) {
  auto vert = Compile(vert_source, GL_VERTEX_SHADER);
  assert(vert != GL_INVALID_VALUE);
  auto frag = Compile(frag_source, GL_FRAGMENT_SHADER);
  assert(frag != GL_INVALID_VALUE);
  GL_CHECK(glAttachShader(program_, vert));
  GL_CHECK(glAttachShader(program_, frag));
  GL_CHECK(glLinkProgram(program_));
  GL_CHECK(glDetachShader(program_, vert));
  GL_CHECK(glDetachShader(program_, frag));

  GLint status;
  glGetProgramiv(program_, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLchar info[512];
    glGetProgramInfoLog(program_, sizeof(info), nullptr, info);
    Log::Error("Failed to link shader: {}", info);
    glDeleteProgram(program_);
    return false;
  }
  return true;
}

void Shader::UploadMatrix2x3(std::string_view name, const real *values) {
  GL_CHECK(glUniformMatrix3x2fv(glGetUniformLocation(program_, name.data()), 1, GL_FALSE, values));
}
