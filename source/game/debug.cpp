// renderer_debug.cpp
#include "system/memory.h"
#include "game/quadtree.h"

#include "glad/glad.h"

namespace Asteroids {
namespace Game {
namespace Debug {

void build_vertex_memory(System::MemoryArena* arena, Game::QTNode* node, int32_t& vertex_count) {
  Math::V3* v = (Math::V3*)System::memory_arena_alloc(arena, 4, sizeof(Math::V3));
  ASSERT(v);

  v[0].x = node->aabb.pos.x - node->aabb.half_edge;
  v[0].y = node->aabb.pos.y + node->aabb.half_edge;
  v[1].x = node->aabb.pos.x - node->aabb.half_edge;
  v[1].y = node->aabb.pos.y - node->aabb.half_edge;
  v[2].x = node->aabb.pos.x + node->aabb.half_edge;
  v[2].y = node->aabb.pos.y - node->aabb.half_edge;
  v[3].x = node->aabb.pos.x + node->aabb.half_edge;
  v[3].y = node->aabb.pos.y + node->aabb.half_edge;

  vertex_count += 4;

  if (node->nw_child) {
    build_vertex_memory(arena, node->nw_child, vertex_count);
  }

  if (node->ne_child) {
    build_vertex_memory(arena, node->ne_child, vertex_count);
  }

  if (node->sw_child) {
    build_vertex_memory(arena, node->sw_child, vertex_count);
  }

  if (node->se_child) {
    build_vertex_memory(arena, node->se_child, vertex_count);
  }
}

void render_quadtree(Game::QuadTree* qt) {
  static uint32_t vao = uint32_t(-1);
  static uint32_t vbo = uint32_t(-1);
  static int32_t vertex_count = 0;

  if (vao == uint32_t(-1) || vbo == uint32_t(-1)) {
    System::MemoryArena* arena = System::memory_arena_create("DBG_V", System::MB(1));
    build_vertex_memory(arena, qt->root_, vertex_count);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Math::V3) * vertex_count, arena->bytes, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Math::V3), (GLvoid*)0);
    glBindVertexArray(0);
  }

  for (int32_t i = 0; i < vertex_count; i += 4) {
    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_LOOP, i, 4);
    glBindVertexArray(0);
  }
}

} //namespace
} //namespace
} //namespace
