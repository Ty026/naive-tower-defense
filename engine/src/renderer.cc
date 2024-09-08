#include "renderer.h"
#include "gl.h"
#include "shader.h"
#include "vertex-array.h"
#include "vertex.h"
#include <numeric>

namespace {
constexpr const int kMaxTriangles = 10000;
constexpr const int kMaxVertices = kMaxTriangles * 3;
constexpr const int kMaxIndices = kMaxTriangles * 3;

int                       next_vertex_index{0};
int                       current_indices_index{0};
Vertex                   *vertices{nullptr};
Vertex                   *vertices_base{nullptr};
int                      *indices{nullptr};
int                       tex_slot_index{0};
Scope<VertexArray>        va;
Scope<Shader>             shader;
Camera2D                 *camera;
std::vector<Ref<Texture>> textures;

int num_bytes() { return (vertices - vertices_base) * sizeof(Vertex); }

inline void BeginBatch() {
  next_vertex_index = 0;
  current_indices_index = 0;
  vertices = vertices_base;
  tex_slot_index = 0;
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

int CalculateTextureIndex(Ref<Texture> texture) {
  int index = -1;
  for (int i = 0; i < tex_slot_index; ++i) {
    if (*textures[i] == *texture) {
      index = i;
      break;
    }
  }
  if (index == -1) {
    index = tex_slot_index;
    textures[index] = texture;
    tex_slot_index++;
  }
  return index;
}

} // namespace

void Renderer::Initialize() {
  int max_texture_units = 0;
  GL_CHECK(glEnable(GL_BLEND));
  GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  GL_CHECK(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units));

  textures.resize(max_texture_units);
  indices = new int[kMaxIndices];
  vertices_base = new Vertex[kMaxVertices];
  va = MakeScope<VertexArray>(kMaxVertices * sizeof(Vertex), kMaxIndices);
  va->set_layout({
      {VertexLayout::DataType::F2},
      {VertexLayout::DataType::RGBA},
      {VertexLayout::DataType::F2},
      {VertexLayout::DataType::I1},
  });
  shader = MakeScope<Shader>(max_texture_units);
  shader->CompileFromFile("assets/basic.vs", "assets/basic.fs");
  shader->Bind();

  std::vector<int> slots(max_texture_units);
  std::iota(slots.begin(), slots.end(), 0);
  shader->UploadIntArray("textures", slots.data(), slots.size());
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
    for (int i = 0; i < tex_slot_index; ++i) {
      textures[i]->Bind(i);
    }
    shader->Bind();
    va->Bind();
    shader->UploadMatrix2x3("view_projection", camera->matrix());
    va->DrawIndexed(vertices_base, num_bytes(), indices, current_indices_index);
  }
}

void Renderer::set_viewport(vec2 size) {
  GL_CHECK(glViewport(0, 0, size.w, size.h));
}

void Renderer::PreDrawCommandCheck(int next_max_indices) {
  bool no_space = current_indices_index + next_max_indices > kMaxIndices;
  if (no_space) {
    Renderer::End();
    BeginBatch();
  }
}

#define ADD_VERTEX(p_pos, p_color, p_tex_index, uv_x, uv_y) \
  {                                                         \
    vertices->position = p_pos;                             \
    vertices->color = p_color;                              \
    vertices->tex_coord.x = uv_x;                           \
    vertices->tex_coord.y = uv_y;                           \
    vertices->tex = p_tex_index;                            \
    vertices++;                                             \
  }

void Renderer::Draw(const vec2 &position, const vec2 &size, const Color &color) {
  PreDrawCommandCheck();
  ADD_VERTEX(position, color, -1, 0, 0);
  ADD_VERTEX((position + vec2(size.w, 0)), color, -1, 0, 0);
  ADD_VERTEX((position + size), color, -1, 0, 0);
  ADD_VERTEX((position + vec2(0, size.h)), color, -1, 0, 0);

  AddQuadIndices();
}

void Renderer::Draw(Ref<Texture> texture, const vec2 &pos, const vec2 &size, const vec2 &uv_min, const vec2 &uv_max, const Color &tint) {
  PreDrawCommandCheck();
  int tex_index = CalculateTextureIndex(texture);
  ADD_VERTEX(pos, tint, tex_index, uv_min.x, uv_min.y);
  ADD_VERTEX(vec2(pos.x + size.w, pos.y), tint, tex_index, uv_max.x, uv_min.y);
  ADD_VERTEX(vec2(pos.x + size.w, pos.y + size.h), tint, tex_index, uv_max.x, uv_max.y);
  ADD_VERTEX(vec2(pos.x, pos.y + size.h), tint, tex_index, uv_min.x, uv_max.y);
  AddQuadIndices();
}
