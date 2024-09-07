#pragma once
#include "gl.h"

class Texture {
public:
  Texture();
  Texture(int w, int h, int channels, const uint8_t *data, bool smooth = false);
  Texture(const Texture &copy) = delete;
  Texture &operator=(const Texture &copy) = delete;
  ~Texture();

  bool operator==(const Texture &other) const { return id_ == other.id_; }

  void Bind(int slot) const;

private:
  GLuint   id_;
  uint32_t data_format_;
  uint32_t internal_format_;
  int      w_{0};
  int      h_{0};
};
