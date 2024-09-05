#pragma once
#include "vec2.h"
#include "color.h"

class Renderer {
public:
  static void set_viewport(vec2 size);

  static void Initialize();
  static void Release();
  static void Clear();
  static void Begin();
  static void End();
  static void Draw(const vec2 &position, const vec2 &size, const Color &color);

private:
  inline static void PreDrawCommandCheck(int next_max_indices = 6);
};
