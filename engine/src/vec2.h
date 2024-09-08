// In this project, the vec2 class is named using lowercase letters,
// which deviates from the typical coding conventions that favor capitalized class names.
// The primary reason for this choice is the extremely high frequency of usage of the
// vec2 class.

#pragma once
#include "common.h"

class vec2 {
public:
  constexpr vec2() : x(0), y(0) {}
  constexpr vec2(real x, real y) : x(x), y(y) {}
  constexpr vec2(real s) : x(s), y(s) {}
  constexpr vec2  operator+(const vec2 &rhs) const { return {x + rhs.x, y + rhs.y}; }
  constexpr vec2  operator*(const vec2 &rhs) const { return {x * rhs.x, y * rhs.y}; }
  constexpr vec2  operator*(real s) const { return {x * s, y * s}; }
  constexpr real &operator[](int index) { return coord[index]; }
  constexpr real  operator[](int index) const { return coord[index]; }
  constexpr vec2  operator-() const { return vec2(-x, -y); }

  union {
    struct {
      union {
        real x;
        real w;
      };
      union {
        real y;
        real h;
      };
    };
    real coord[2];
  };
};
