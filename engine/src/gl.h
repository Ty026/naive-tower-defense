#pragma once

#include "glad/glad.h"

#if !defined(NDEBUG)

namespace internal {
void CheckGLError(const char *file, int line, const char *expr);
}

#define GL_CHECK(expr)                                 \
  do {                                                 \
    expr;                                              \
    internal::CheckGLError(__FILE__, __LINE__, #expr); \
  } while (0)
#else
#define GL_CHECK(expr) expr
#endif
