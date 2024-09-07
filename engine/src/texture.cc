#include "texture.h"

Texture::Texture() {
  GL_CHECK(glGenTextures(1, &id_));
}

Texture::~Texture() {
  GL_CHECK(glDeleteTextures(1, &id_));
}

Texture::Texture(int w, int h, int channels, const uint8_t *data, bool smooth) : w_(w), h_(h) {
  if (channels == 1) {
    internal_format_ = GL_R8;
    data_format_ = GL_RED;
  } else if (channels == 3) {
    internal_format_ = GL_RGBA8;
    data_format_ = GL_RGB;
  } else {
    internal_format_ = GL_RGBA8;
    data_format_ = GL_RGBA;
  }
  GL_CHECK(glGenTextures(1, &id_));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, id_));
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, internal_format_, w, h, 0, data_format_, GL_UNSIGNED_BYTE, nullptr));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR : GL_NEAREST));
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR : GL_NEAREST));
  GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, data_format_, GL_UNSIGNED_BYTE, data));
}

void Texture::Bind(int slot) const {
  GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, id_));
}
