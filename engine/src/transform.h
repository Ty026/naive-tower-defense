#pragma once

#include "vec2.h"
class Transform {
public:
  Transform() { Identity(); }
  constexpr void Identity();

  const real *value_ptr() const { return &columns_[0].x; }
  void        set_rotation_and_scale(real rotation, const vec2 &scale);

  void Ortho(real left, real right, real bottom, real top);
  void Translate(const vec2 &translation);

  vec2     &operator[](int index) { return columns_[index]; }
  void      operator*=(const Transform &right);
  Transform operator*(const Transform &rhs) const;
  vec2      operator*(const vec2 &v) const;

private:
  vec2 columns_[3];
};

constexpr void Transform::Identity() {
  columns_[0] = {1, 0};
  columns_[1] = {0, 1};
  columns_[2] = {0, 0};
}
