#include "shader.h"
#include "log.h"
#include "file-reader.h"
#include <cassert>
#include <regex>

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

Shader::Shader(int max_texture_units) : max_texture_units_(max_texture_units) {
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
  auto frag = Compile(GenerateFSCodeFromSource(frag_source), GL_FRAGMENT_SHADER);
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

void Shader::UploadIntArray(const std::string_view name, const int *values, int count) {
  int32_t location;
  GL_CHECK(location = glGetUniformLocation(program_, name.data()));
  GL_CHECK(glUniform1iv(location, count, values));
}

// this function generates code by replacing two placeholders in the template:
// 1. `/*$max_textures*/` is replaced with the actual number of texture units
// 2. `/*each in textures*/` is replaced with a series of `switch` statements
std::string Shader::GenerateFSCodeFromSource(std::string_view template_code) {
  // TODO: maybe a simple parser for generating this code
  std::string code = std::regex_replace(
      template_code.data(),
      std::regex(R"(/\*\$max_textures\*/)", std::regex_constants::ECMAScript),
      std::to_string(max_texture_units_));
  std::stringstream cases;
  for (int i = 0; i < max_texture_units_; ++i) {
    cases << "case " << i << ":\n"
          << "    FragColor = texture(textures[" << i << "], tex_coord);\n"
          << "    break;\n";
  }

  code = std::regex_replace(
      code, std::regex(R"(/\*each in textures\*/)", std::regex_constants::ECMAScript),
      cases.str());
  return code;
}
