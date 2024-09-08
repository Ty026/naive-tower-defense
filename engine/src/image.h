#pragma once
#include "common.h"
#include <cstdint>
struct Image {
  int      w;
  int      h;
  int      channels;
  uint8_t *data;
};

struct ImageLoader {
  Ref<Image> operator()(std::string_view path);
};
