#pragma once
#include "texture.h"
#include "vec2.h"
#include "color.h"
#include "camera-2d.h"

class Renderer {
public:
  static void set_viewport(vec2 size);

  static void Initialize();
  static void Release();
  static void Clear();
  static void Begin(Camera2D &c);
  static void End();
  static void Draw(const vec2 &position, const vec2 &size, const Color &color);
  static void Draw(Ref<Texture> texture, const vec2 &position, const vec2 &size, const vec2 &uv_min = {0, 0}, const vec2 &uv_max = {1, 1}, const Color &tint = Color::White);
  static void Draw(Ref<Texture> texture, const vec2 &size, const Transform &, const vec2 &uv_min, const vec2 &uv_max, const vec2 &origin, const Color &tint = Color::White);

private:
  inline static void PreDrawCommandCheck(int next_max_indices = 6);
};
