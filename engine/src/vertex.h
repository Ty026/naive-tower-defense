#pragma once

#include "color.h"
#include "vec2.h"

struct Vertex {
  vec2  position;
  Color color;
  vec2  tex_coord;
  int   tex;
};
