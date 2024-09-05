#pragma once

#include <cstdint>
#include <initializer_list>
#include <vector>
#include "gl.h"

class VertexLayout {
public:
  // clang-format off
  enum class DataType { F1, F2, F3, F4, I1, I2, I3, I4, U1, U2, U3, U4, RGBA };
  // clang-format on
  struct Attribute {
    constexpr Attribute(DataType type, bool normalized = false);
    int      size;
    int      num_bytes;
    uint32_t gl_type;
    int      offset{0};
    int      normalized{0};
    bool     is_integer{false};
  };

  VertexLayout() = default;
  const std::vector<Attribute>          &attributes() const { return attributes_; }
  std::vector<Attribute>::const_iterator begin() const { return attributes_.begin(); }
  std::vector<Attribute>::const_iterator end() const { return attributes_.end(); }
  const int                              stride() const { return stride_; }

  VertexLayout(const std::initializer_list<Attribute> &attributes) : attributes_(attributes) {
    int offset{0};
    for (auto &attr : attributes_) {
      attr.offset = offset;
      offset += attr.num_bytes;
      stride_ += attr.num_bytes;
    }
  }

private:
  std::vector<Attribute> attributes_;
  int                    stride_{0};
};

namespace {
constexpr int GetAttributeSize(VertexLayout::DataType type) {
  switch (type) {
  case VertexLayout::DataType::F1: return 1;
  case VertexLayout::DataType::F2: return 2;
  case VertexLayout::DataType::F3: return 3;
  case VertexLayout::DataType::F4: return 4;
  case VertexLayout::DataType::I1: return 1;
  case VertexLayout::DataType::I2: return 2;
  case VertexLayout::DataType::I3: return 3;
  case VertexLayout::DataType::I4: return 4;
  case VertexLayout::DataType::U1: return 1;
  case VertexLayout::DataType::U2: return 2;
  case VertexLayout::DataType::U3: return 3;
  case VertexLayout::DataType::U4: return 4;
  case VertexLayout::DataType::RGBA: return 4;
  };
}

constexpr int GetAttributeNumBytes(VertexLayout::DataType type) {
  switch (type) {
  case VertexLayout::DataType::F1: return 1 * 4;
  case VertexLayout::DataType::F2: return 2 * 4;
  case VertexLayout::DataType::F3: return 3 * 4;
  case VertexLayout::DataType::F4: return 4 * 4;
  case VertexLayout::DataType::I1: return 1 * 4;
  case VertexLayout::DataType::I2: return 2 * 4;
  case VertexLayout::DataType::I3: return 3 * 4;
  case VertexLayout::DataType::I4: return 4 * 4;
  case VertexLayout::DataType::U1: return 1 * 4;
  case VertexLayout::DataType::U2: return 2 * 4;
  case VertexLayout::DataType::U3: return 3 * 4;
  case VertexLayout::DataType::U4: return 4 * 4;
  case VertexLayout::DataType::RGBA: return 4;
  };
}

constexpr uint32_t GetAttributeGLType(VertexLayout::DataType type) {
  switch (type) {
  case VertexLayout::DataType::F1: return GL_FLOAT;
  case VertexLayout::DataType::F2: return GL_FLOAT;
  case VertexLayout::DataType::F3: return GL_FLOAT;
  case VertexLayout::DataType::F4: return GL_FLOAT;
  case VertexLayout::DataType::I1: return GL_INT;
  case VertexLayout::DataType::I2: return GL_INT;
  case VertexLayout::DataType::I3: return GL_INT;
  case VertexLayout::DataType::I4: return GL_INT;
  case VertexLayout::DataType::U1: return GL_UNSIGNED_INT;
  case VertexLayout::DataType::U2: return GL_UNSIGNED_INT;
  case VertexLayout::DataType::U3: return GL_UNSIGNED_INT;
  case VertexLayout::DataType::U4: return GL_UNSIGNED_INT;
  case VertexLayout::DataType::RGBA: return GL_UNSIGNED_BYTE;
  };
}
} // namespace

constexpr VertexLayout::Attribute::Attribute(VertexLayout::DataType type, bool norm) {
  size = GetAttributeSize(type);
  num_bytes = GetAttributeNumBytes(type);
  gl_type = GetAttributeGLType(type);
  normalized = type == VertexLayout::DataType::RGBA ? GL_TRUE : (norm ? GL_TRUE : GL_FALSE);
  if (gl_type == GL_INT || gl_type == GL_UNSIGNED_INT) {
    is_integer = true;
  }
}
