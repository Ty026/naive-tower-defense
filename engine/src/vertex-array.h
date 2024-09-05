#pragma once
#include "gl.h"
#include "vertex-layout.h"

class VertexArray {
public:
  VertexArray(int vertices_size, int max_index_count);
  ~VertexArray();
  void DrawIndexed(void *vertices, int vertex_size, int *indices, int index_count);
  void set_layout(const VertexLayout &layout);
  void Bind();

private:
  GLuint vao{0};
  GLuint vbo{0};
  GLuint ibo{0};
};
