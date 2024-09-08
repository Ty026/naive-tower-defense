#include "transform.h"
#include <cmath>

// This is a simplified version of an orthographic projection matrix
// z-axis is not considered as we only care about 2D transformations
void Transform::Ortho(real left, real right, real bottom, real top) {
  columns_[0] = {2.f / (right - left), 0};
  columns_[1] = {0, 2.f / (top - bottom)};
  columns_[2] = {-(right + left) / (right - left), -(top + bottom) / (top - bottom)};
}

// This is effectively performing the matrix multiplication:
// [ a, b, 0 ]   [1, 0, tx]
// [ c, d, 0 ] * [0, 1, ty]
// [ 0, 0, 1 ]   [0, 0, 1 ]
void Transform::Translate(const vec2 &translation) {
  columns_[2][0] += columns_[0][0] * translation.x + columns_[1][0] * translation.y;
  columns_[2][1] += columns_[0][1] * translation.x + columns_[1][1] * translation.y;
}

// Sets the rotation and scale for the transformation matrix.
// [ cos(θ) * scale.x   -sin(θ) * scale.y   0 ]
// [ sin(θ) * scale.x    cos(θ) * scale.y   0 ]
//
// It does not perform a full matrix multiplication, as it is intended only
// to update the rotation and scale components of the transformation.
void Transform::set_rotation_and_scale(real rotation, const vec2 &scale) {
  real cos = std::cos(rotation);
  real sin = std::sin(rotation);
  columns_[0][0] = cos * scale.x;
  columns_[0][1] = sin * scale.x;
  columns_[1][0] = -sin * scale.y;
  columns_[1][1] = cos * scale.y;
}

Transform Transform::operator*(const Transform &rhs) const {
  Transform result = *this;
  result *= rhs;
  return result;
}

void Transform::operator*=(const Transform &right) {
  auto &a = columns_;
  auto &b = right.columns_;
  columns_[2].x += a[0].x * b[2].x + a[1].x * b[2].y;
  columns_[2].y += a[0].y * b[2].x + a[1].y * b[2].y;
  real x0 = a[0].x * b[0].x + a[1].x * b[0].y;
  real x1 = a[0].y * b[0].x + a[1].y * b[0].y;
  real y0 = a[0][0] * b[1].x + a[1][0] * b[1].y;
  real y1 = a[0][1] * b[1].x + a[1][1] * b[1].y;
  columns_[0][0] = x0;
  columns_[0][1] = x1;
  columns_[1][0] = y0;
  columns_[1][1] = y1;
}

vec2 Transform::operator*(const vec2 &v) const {
  return {columns_[0].x * v.x + columns_[1].x * v.y + columns_[2].x,
          columns_[0].y * v.x + columns_[1].y * v.y + columns_[2].y};
}
