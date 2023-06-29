// mesh.h
#pragma once

#include "system/fileio.h"
#include "math/vector3.h"
#include "math/matrix4.h"
#include "math/aabb.h"

namespace Asteroids {
namespace Rendering {

struct Vertex {
  Math::V3 position;
  //Math::V3 normal;
};

struct Triangle {
  uint32_t a, b, c;
};

struct TriangleMesh {
  Vertex* vertices;
  size_t vertex_count;
  Triangle* triangles;
  size_t triangle_count;
};

class MeshBuilder final {
  DISABLE_COPY_AND_MOVE(MeshBuilder);
public:
  MeshBuilder() = default;
  ~MeshBuilder() = default;

  bool init(System::MemoryArena* arena, size_t max_mesh_count);
  void finalize();

  TriangleMesh* mesh_from_obj(const System::ByteBuffer* buffer, size_t max_vertex_count, size_t max_triangle_count);
private:
  System::MemoryArena* mesh_arena = nullptr;

  TriangleMesh* mesh_list_ = nullptr;
  size_t mesh_max_count_ = 0;
  size_t mesh_used_count_ = 0;
};

} //namespace
} //namespace
