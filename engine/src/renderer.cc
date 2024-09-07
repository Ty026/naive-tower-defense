#include "renderer.h"
#include "gl.h"
#include "shader.h"
#include "vertex-array.h"
#include "vertex.h"

namespace {
constexpr const int kMaxTriangles = 10000;
constexpr const int kMaxVertices = kMaxTriangles * 3;
constexpr const int kMaxIndices = kMaxTriangles * 3;

int                next_vertex_index{0};
int                current_indices_index{0};
Vertex            *vertices{nullptr};
Vertex            *vertices_base{nullptr};
int               *indices{nullptr};
Scope<VertexArray> va;
Scope<Shader>      shader;
Camera2D          *camera;

int num_bytes() { return (vertices - vertices_base) * sizeof(Vertex); }

inline void BeginBatch() {
  next_vertex_index = 0;
  current_indices_index = 0;
  vertices = vertices_base;
}

inline void AddQuadIndices() {
  indices[current_indices_index + 0] = next_vertex_index + 0;
  indices[current_indices_index + 1] = next_vertex_index + 1;
  indices[current_indices_index + 2] = next_vertex_index + 2;
  indices[current_indices_index + 3] = next_vertex_index + 0;
  indices[current_indices_index + 4] = next_vertex_index + 2;
  indices[current_indices_index + 5] = next_vertex_index + 3;
  next_vertex_index += 4;
  current_indices_index += 6;
}

} // namespace

void Renderer::Initialize() {
  GL_CHECK(glEnable(GL_BLEND));
  GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  indices = new int[kMaxIndices];
  vertices_base = new Vertex[kMaxVertices];
  va = MakeScope<VertexArray>(kMaxVertices * sizeof(Vertex), kMaxIndices);
  va->set_layout({
      {VertexLayout::DataType::F2},
      {VertexLayout::DataType::RGBA},
  });
  shader = MakeScope<Shader>();
  shader->CompileFromFile("assets/basic.vs", "assets/basic.fs");
  shader->Bind();
}

void Renderer::Release() {
  shader = nullptr;
  va = nullptr;
  delete[] vertices_base;
  delete[] indices;
}

void Renderer::Clear() {
  GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Begin(Camera2D &p_camera) {
  camera = &p_camera;
  camera->BuildMatrix();
  BeginBatch();
}

void Renderer::End() {
  if (current_indices_index > 0) {
    shader->Bind();
    va->Bind();
    shader->UploadMatrix2x3("view_projection", camera->matrix());
    va->DrawIndexed(vertices_base, num_bytes(), indices, current_indices_index);
  }
}

void Renderer::PreDrawCommandCheck(int next_max_indices) {
  bool no_space = current_indices_index + next_max_indices > kMaxIndices;
  if (no_space) {
    Renderer::End();
    BeginBatch();
  }
}

#define ADD_VERTEX(p_pos, p_color) \
  {                                \
    vertices->position = p_pos;    \
    vertices->color = p_color;     \
    vertices++;                    \
  }

void Renderer::Draw(const vec2 &position, const vec2 &size, const Color &color) {
  PreDrawCommandCheck();
  ADD_VERTEX(position, color);
  ADD_VERTEX((position + vec2(size.w, 0)), color);
  ADD_VERTEX((position + size), color);
  ADD_VERTEX((position + vec2(0, size.h)), color);

  AddQuadIndices();
}

void Renderer::set_viewport(vec2 size) {
  GL_CHECK(glViewport(0, 0, size.w, size.h));
}
