#include "renderer.h"
#include "gl.h"
void Renderer::set_viewport(vec2 size) {
  GL_CHECK(glViewport(0, 0, size.w, size.h));
}
