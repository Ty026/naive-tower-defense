#pragma once

#include "vec2.h"
#include "transform.h"

class Camera2D {
public:
  virtual ~Camera2D() = default;
  virtual void BuildMatrix();

  const real *matrix() const { return mvp_.value_ptr(); }
  void        set_viewport(const vec2 &size);

protected:
  Transform projection_;
  Transform mvp_;
  Transform view_;
  vec2      offset_;
  vec2      target_;
  real      rotation_{0};
  real      zoom_{1};
};
