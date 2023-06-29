// renderer.h
#pragma once
#include "system/memory.h"

#include "math/vector3.h"
#include "math/matrix4.h"
#include "math/aabb.h"

#include "rendering/mesh.h"

namespace Asteroids {
namespace Rendering {

using namespace Math;

//FIXME:
struct GlVertexArray {
  uint32_t vao;
  uint32_t vbo;
  uint32_t ebo;
  size_t element_count;
};

class Renderer final {
  DISABLE_COPY_AND_MOVE(Renderer);
public:
  Renderer() = default;
  ~Renderer() = default;
  bool init(System::MemoryArena* arena);
  void finalize();
  
  bool init_window(int width, int height, bool fullscreen);
  bool init_opengl();
  //FIXME: bool init_shader_program_list(size_t max_shader_program_count);
  bool init_vertex_array_list(size_t max_vertex_array_count);
  //FIXME: bool init_texture_list(size_t max_texture_count);

  int window_width() { return window_width_; }
  int window_height() { return window_height_; }

  uint32_t build_shader_program(
    const System::ByteBuffer* vertex_shader,
    const System::ByteBuffer* fragment_shader
  );

  int32_t build_vertex_array(const TriangleMesh* mesh);

  int32_t shader_uniform_location(uint32_t shader_handle, const char* uniform_name);
  void shader_set_uniform(int32_t uniform_location, const Math::M4& m);

  void begin_frame();
  void end_frame();

  void use_texture(uint32_t texture_id);
  void use_shader_program(uint32_t shader_handle);

  void render_vertex_array(size_t index);
  void render_aabb(const AABB& aabb);

private:
  int window_width_ = 800;
  int window_height_ = 600;
  SDL_Window* window_ = nullptr;
  SDL_GLContext gl_context_ = nullptr;

  System::MemoryArena* renderer_arena = nullptr;

  GlVertexArray* vertex_array_list_ = nullptr;
  size_t vertex_array_max_count_ = 0;
  size_t vertex_array_used_count_ = 0;

  //Debug
  uint32_t aabb_vao_ = uint32_t(-1);
  uint32_t aabb_vbo_ = uint32_t(-1);
};

} //namespace
} //namespace
