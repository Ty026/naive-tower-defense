#include "color.h"

Color Color::FromRGBA(uint32_t rbga) {
  uint8_t a = rbga & 0xFF;
  rbga >>= 8;
  uint8_t b = rbga & 0xFF;
  rbga >>= 8;
  uint8_t g = rbga & 0xFF;
  rbga >>= 8;
  uint8_t r = rbga & 0xFF;
  return Color(r, g, b, a);
}

Color Color::FromRGB(uint32_t rgb) {
  uint8_t b = rgb & 0xFF;
  rgb >>= 8;
  uint8_t g = rgb & 0xFF;
  rgb >>= 8;
  uint8_t r = rgb & 0xFF;
  return Color(r, g, b, 255);
}
