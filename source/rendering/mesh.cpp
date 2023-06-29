// mesh.cpp
#include "mesh.h"

namespace Asteroids {
namespace Rendering {

constexpr size_t MAX_LINE_LENGTH = 256;

bool MeshBuilder::init(System::MemoryArena* arena, size_t max_mesh_count) {
  ASSERT(arena && arena->allocated_size > 0);
  if (!arena || !arena->allocated_size) {
    return false;
  }

  mesh_arena = arena;

  mesh_list_ = (TriangleMesh*)System::memory_arena_alloc(mesh_arena, max_mesh_count, sizeof(TriangleMesh));
  if (!mesh_list_) {
    return false;
  }

  mesh_max_count_ = max_mesh_count;
  return true;
}

void MeshBuilder::finalize() {
}

TriangleMesh* MeshBuilder::mesh_from_obj(const System::ByteBuffer* buffer, size_t max_vertex_count, size_t max_triangle_count) {
  ASSERT(is_valid(buffer));
  ASSERT(mesh_used_count_ < mesh_max_count_);

  if (mesh_used_count_ >= mesh_max_count_) {
    System::log_error("Mesh list is too small [%u]", mesh_max_count_);
    return nullptr;
  }

  TriangleMesh* mesh = &mesh_list_[mesh_used_count_];
  mesh->vertices = (Vertex*)System::memory_arena_alloc(mesh_arena, max_vertex_count, sizeof(Vertex));
  mesh->triangles = (Triangle*)System::memory_arena_alloc(mesh_arena, max_triangle_count, sizeof(Triangle));

  const char* buffer_end = (char*)buffer->bytes + buffer->size;
  const char* line_start = (char*)buffer->bytes;
  size_t line_len = 0;
  char line[MAX_LINE_LENGTH];

  size_t vertex_position_count = 0;
  size_t vertex_normal_count = 0;

  while (line_start + line_len < buffer_end) {
    line_start += line_len;
    line_len = 0;

    while (*(line_start + line_len) != '\n' && line_start + line_len < buffer_end) {
      line_len++;
    }
    line_len++;

    if (*line_start == '#') {
      continue;
    }

    memset(line, 0, MAX_LINE_LENGTH);
    memcpy(line, line_start, System::min(line_len - 1, MAX_LINE_LENGTH - 1));

    if (strlen(line) == 0) {
      continue;
    }

    if (strncmp(line, "v ", 2) == 0) {
      if (vertex_position_count < max_vertex_count) {
        float x, y, z;
        if (sscanf(line, "v %f %f %f\n", &x, &y, &z) == 3) {
          mesh->vertices[vertex_position_count].position.x = x;
          mesh->vertices[vertex_position_count].position.y = y;
          mesh->vertices[vertex_position_count].position.z = z;
          vertex_position_count++;
        }
      } else {
        System::log_error("Mesh vertex position array is too small [%u]", max_vertex_count);
        return nullptr;
      }
   /* } else if (strncmp(line, "vn ", 3) == 0) {
      if (vertex_normal_count < max_vertex_count) {
        float x, y, z;
        if (sscanf(line, "vn %f %f %f\n", &x, &y, &z) == 3) {
          mesh->vertices[vertex_normal_count].normal.x = x;
          mesh->vertices[vertex_normal_count].normal.y = y;
          mesh->vertices[vertex_normal_count].normal.z = z;
          vertex_normal_count++;
        }
      } else {
        System::log_error("Mesh vertex normal array is too small [%u]", max_vertex_count);
        return nullptr;
      }*/
    /*
    } else if (strncmp(line, "vt ", 3) == 0) {
      float f[2] = {0.0f};

      if (sscanf(line, "vt %f %f\n", &f[0], &f[1]) == 2) {
        Vec2 uv = {.x = f[0], .y = 1.0f - f[1]};
        dyn_array_push_back(mesh->uvs, uv);
      }*/

    } else if (strncmp(line, "f ", 2) == 0) {
      if (mesh->triangle_count < max_triangle_count) {
        int a, b, c;
        int uv_a, uv_b, uv_c;
        int n_a, n_b, n_c;

        if (sscanf(line, "f %i/%i/%i\t%i/%i/%i\t%i/%i/%i\n",
          &a, &uv_a, &n_a, &b, &uv_b, &n_b, &c, &uv_c, &n_c) == 9) {

          mesh->triangles[mesh->triangle_count].a = a - 1;
          mesh->triangles[mesh->triangle_count].b = b - 1;
          mesh->triangles[mesh->triangle_count].c = c - 1;
          mesh->triangle_count++;
        }
      } else {
        System::log_error("Mesh triangle array is too small [%u]", max_triangle_count);
        return nullptr;
      }
    }
  }

  //ASSERT(vertex_position_count == vertex_normal_count);
  mesh->vertex_count = vertex_position_count;

  mesh_used_count_++;
  return mesh;
}

} //namespace
} //namespace
