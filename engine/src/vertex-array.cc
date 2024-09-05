#include "vertex-array.h"
#include "log.h"
#include "vertex.h"

VertexArray::VertexArray(int vertices_size, int max_index_count) {
  GL_CHECK(glGenVertexArrays(1, &vao));
  GL_CHECK(glBindVertexArray(vao));
  GL_CHECK(glGenBuffers(1, &ibo));
  GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
  GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_index_count * sizeof(int), nullptr, GL_STATIC_DRAW));
  GL_CHECK(glGenBuffers(1, &vbo));
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
  GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertices_size, nullptr, GL_STATIC_DRAW));
}

void VertexArray::Bind() {
  GL_CHECK(glBindVertexArray(vao));
}

VertexArray::~VertexArray() {
  GL_CHECK(glDeleteBuffers(1, &ibo));
  GL_CHECK(glDeleteBuffers(1, &vbo));
  GL_CHECK(glDeleteVertexArrays(1, &ibo));
}

void VertexArray::DrawIndexed(void *vertices, int vertex_size, int *indices, int index_count) {
  GL_CHECK(glBindVertexArray(vao));
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
  GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_size, vertices));
  GL_CHECK(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, index_count * sizeof(int), indices));
  GL_CHECK(glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr));
}

void VertexArray::set_layout(const VertexLayout &layout) {
  for (int i = 0; auto &attr : layout) {
    GL_CHECK(glEnableVertexAttribArray(i));
    if (attr.is_integer) {
      GL_CHECK(glVertexAttribIPointer(i, attr.size, attr.gl_type, layout.stride(), reinterpret_cast<void *>(attr.offset)));
    } else {
      GL_CHECK(glVertexAttribPointer(i, attr.size, attr.gl_type, attr.normalized, layout.stride(), reinterpret_cast<void *>(attr.offset)));
    }
    i++;
  }
}
