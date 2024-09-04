#include "gl.h"
#include "log.h"

void internal::CheckGLError(const char *file, int line, const char *expr) {
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::Error("GL error {}:{}: {}", file, line, expr);
    exit(1);
  }
}
