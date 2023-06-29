// display.cpp
#include "Renderer.h"

#include <glad/glad.h>

namespace Asteroids {
namespace Rendering {

bool Renderer::init(System::MemoryArena* arena) {
  ASSERT(arena && arena->allocated_size > 0);
  if (!arena || !arena->allocated_size) {
    return false;
  }

  renderer_arena = arena;
  return true;
}

void Renderer::finalize() {

  if (vertex_array_list_ && vertex_array_used_count_ > 0) {
    for (int i = 0; i < vertex_array_used_count_; i++) {
      glDeleteVertexArrays(1, (GLuint*)&vertex_array_list_[i].ebo);
      glDeleteVertexArrays(1, (GLuint*)&vertex_array_list_[i].vbo);
      glDeleteVertexArrays(1, (GLuint*)&vertex_array_list_[i].vao);
    }
  }

}

bool Renderer::init_window(int width, int height, bool fullscreen) {
  if (width > 0 && height > 0) {
    window_width_ = width;
    window_height_ = height;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  if (fullscreen) {
    window_ = SDL_CreateWindow("", 0, 0, window_width_, window_height_,
      SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_INPUT_GRABBED);
  } else {
    window_ = SDL_CreateWindow("Asteroids", 0, 0, window_width_, window_height_, SDL_WINDOW_OPENGL);
  }

  if (window_ == nullptr) {
    System::log_error(SDL_GetError());
    return false;
  }

  gl_context_ = SDL_GL_CreateContext(window_);
  if (gl_context_ == nullptr) {
    System::log_error(SDL_GetError());
    return false;
  }

  SDL_SetWindowPosition(window_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  return true;
}

bool Renderer::init_opengl() {
  if (!gladLoadGL()) {
    System::log_error("Glad failed to load GL!");
    return false;
  }

  System::log_info("Renderer: %s", glGetString(GL_RENDERER));
  System::log_info("OpenGL Version: %s", glGetString(GL_VERSION));
  System::log_info("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

  glViewport(0, 0, window_width_, window_height_);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  
  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glFrontFace(GL_CW);

  return true;
}

bool Renderer::init_vertex_array_list(size_t max_vertex_array_count) {
  ASSERT(renderer_arena);
  vertex_array_list_ = (GlVertexArray*)System::memory_arena_alloc(renderer_arena, max_vertex_array_count, sizeof(GlVertexArray));
  if (vertex_array_list_) {
    vertex_array_max_count_ = max_vertex_array_count;
    return true;
  }

  return false;
}

uint32_t Renderer::build_shader_program(
  const System::ByteBuffer* vertex_shader,
  const System::ByteBuffer* fragment_shader) {

  //ASSERT(vertex_shader && vertex_shader->bytes && vertex_shader->size);
  //ASSERT(fragment_shader && fragment_shader->bytes && fragment_shader->size);

  uint32_t program_handle = glCreateProgram();
  ASSERT(program_handle > 0);
  GLint success = 0;

  const GLuint vertex_shader_handle = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader_handle, 1, (const GLchar* const*)&vertex_shader->bytes, (const GLint*)&vertex_shader->size);
  glCompileShader(vertex_shader_handle);
  glGetShaderiv(vertex_shader_handle, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar info[512] = {};
    glGetShaderInfoLog(vertex_shader_handle, 512, nullptr, info);
    System::log_error("Failed to compile vertex shader!\n%s", info);
    return program_handle;
  }

  const GLuint fragment_shader_handle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader_handle, 1, (const GLchar* const*)&fragment_shader->bytes, (const GLint*)&fragment_shader->size);
  glCompileShader(fragment_shader_handle);
  glGetShaderiv(fragment_shader_handle, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar info[512] = {};
    glGetShaderInfoLog(fragment_shader_handle, 512, nullptr, info);
    System::log_error("Failed to compile fragment shader!\n%s", info);
    return program_handle;
  }
  
  glAttachShader(program_handle, vertex_shader_handle);
  glAttachShader(program_handle, fragment_shader_handle);
  glLinkProgram(program_handle);
  glGetProgramiv(program_handle, GL_LINK_STATUS, &success);
  if (!success) {
    GLchar info[512];
    glGetProgramInfoLog(program_handle, 512, nullptr, info);
    System::log_error("Failed to link shader program!\n%s", info);
  }

  glDetachShader(program_handle, vertex_shader_handle);
  glDeleteShader(vertex_shader_handle);
  glDetachShader(program_handle, fragment_shader_handle);
  glDeleteShader(fragment_shader_handle);
  return program_handle;
}

int32_t Renderer::shader_uniform_location(uint32_t shader_handle, const char* uniform_name) {
  GLint loc = glGetUniformLocation(shader_handle, &uniform_name[0]);
  auto error = glGetError();
  ASSERT(error != GL_INVALID_VALUE);
  ASSERT(error != GL_INVALID_OPERATION);
  return loc;
}

void Renderer::shader_set_uniform(int32_t uniform_location, const Math::M4& m) {
  glUniformMatrix4fv(uniform_location, 1, GL_FALSE, m.m);
}

int32_t Renderer::build_vertex_array(const TriangleMesh* mesh) {
  ASSERT(mesh);
  ASSERT(vertex_array_list_ && vertex_array_max_count_);
  ASSERT(vertex_array_used_count_ + 1 <= vertex_array_max_count_);

  GlVertexArray va = {};
  const size_t vertex_array_index = vertex_array_used_count_;

  glGenVertexArrays(1, &va.vao);
  glGenBuffers(1, &va.vbo);
  glGenBuffers(1, &va.ebo);
  
  glBindVertexArray(va.vao);
  glBindBuffer(GL_ARRAY_BUFFER, va.vbo);
  glBufferData(GL_ARRAY_BUFFER, mesh->vertex_count * sizeof(Vertex), mesh->vertices, GL_STATIC_DRAW); // FIXME: GL_STATIC_DRAW
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
  //  glEnableVertexAttribArray(1);
  //  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
  //
  //  glEnableVertexAttribArray(2);
  //  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, color)));
  //
  //
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, va.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->triangle_count * sizeof(uint32_t), mesh->triangles, GL_STATIC_DRAW); // FIXME:
  glBindVertexArray(0);

  va.element_count = mesh->triangle_count * 3;

  vertex_array_list_[vertex_array_index] = va;
  vertex_array_used_count_++;
  
  return vertex_array_index;
}

void Renderer::begin_frame() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::end_frame() {
  SDL_GL_SwapWindow(window_);
}

void Renderer::use_texture(uint32_t texture_id) {

}

void Renderer::use_shader_program(uint32_t shader_handle) {
  glUseProgram(shader_handle);
}

void Renderer::render_vertex_array(size_t index) {
  ASSERT(index < vertex_array_used_count_);

  glBindVertexArray(vertex_array_list_[index].vao);
  glDrawElements(GL_TRIANGLES, vertex_array_list_[index].element_count, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Renderer::render_aabb(const AABB& box) {

  if (aabb_vao_ == uint32_t(-1) || aabb_vbo_ == uint32_t(-1)) {
    glGenVertexArrays(1, &aabb_vao_);
    glGenBuffers(1, &aabb_vbo_);
    glBindVertexArray(aabb_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, aabb_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Math::V3) * 8, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glBindVertexArray(0);
  }

  const Math::V3 vertex_data[8] = {
    {box.pos.x - box.half_edge, box.pos.y - box.half_edge, box.pos.z + box.half_edge},
    {box.pos.x - box.half_edge, box.pos.y + box.half_edge, box.pos.z + box.half_edge},
    {box.pos.x + box.half_edge, box.pos.y + box.half_edge, box.pos.z + box.half_edge},
    {box.pos.x + box.half_edge, box.pos.y - box.half_edge, box.pos.z + box.half_edge},
    {box.pos.x - box.half_edge, box.pos.y - box.half_edge, box.pos.z - box.half_edge},
    {box.pos.x - box.half_edge, box.pos.y + box.half_edge, box.pos.z - box.half_edge},
    {box.pos.x + box.half_edge, box.pos.y + box.half_edge, box.pos.z - box.half_edge},
    {box.pos.x + box.half_edge, box.pos.y - box.half_edge, box.pos.z - box.half_edge},
  };

  glBindBuffer(GL_ARRAY_BUFFER, aabb_vbo_);
  void* vbo_ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  memcpy(vbo_ptr, vertex_data, sizeof(vertex_data));
  glUnmapBuffer(GL_ARRAY_BUFFER);

  glBindVertexArray(aabb_vao_);
  glDrawArrays(GL_LINE_LOOP, 0, 8);
  glBindVertexArray(0);
}

} //namespace
} //namespace
