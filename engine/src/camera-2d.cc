#include "camera-2d.h"

void Camera2D::BuildMatrix() {
  view_.Identity();
  view_.Translate(offset_);
  view_.set_rotation_and_scale(rotation_, {zoom_, zoom_});
  mvp_ = projection_ * view_;
}

void Camera2D::set_viewport(const vec2 &size) {
  projection_.Ortho(0, size.w, size.h, 0);
}
