// setup.h
#pragma once

#include "system/system.h"
#include "system/memory.h"
#include "system/config.h"

#include "game/input.h"
#include "game/ecs.h"

#include "rendering/mesh.h"
#include "rendering/renderer.h"

#include "audio/sound.h"

namespace Asteroids {
namespace Game {

struct EntityData {
  Rendering::TriangleMesh* mesh;
  int vertex_array_idx;
  int sound_indecies[MAX_SOUND_COMPONENT_SOUNDS];
};

class Global final {
  DISABLE_COPY_AND_MOVE(Global);
public:
  Global();
  ~Global();

  static const size_t FILE_IO_ARENA_SIZE;
  static const size_t MESH_ARENA_SIZE;
  static const size_t RENDERER_ARENA_SIZE;
  static const size_t ENTITY_ARENA_SIZE;
  static const size_t QUADTREE_ARENA_SIZE;

  static const size_t MAX_MESH_COUNT;
  static const size_t MAX_VERTEX_ARRAY_COUNT;
  static const size_t MAX_TEXTURE_COUNT;
  static const size_t MAX_ENTITY_COUNT;

  static const float WORLD_HALF_EDGE;

  System::MemoryArena* file_io_arena = nullptr;
  System::MemoryArena* mesh_arena = nullptr;
  System::MemoryArena* renderer_arena = nullptr;
  System::MemoryArena* entity_arena = nullptr;
  System::MemoryArena* quadtree_arena = nullptr;

  Game::InputHandler input;
  Rendering::Renderer renderer;
  Rendering::MeshBuilder mesh_builder;
  Audio::SoundPlayer sound_player;
  Game::EntityComponentList entity_list;

  uint32_t main_shader_handle = 0;
  uint32_t player_ui_shader_handle = 0;
  uint32_t menu_shader_handle = 0;

  EcsId player_entity_id = ECSID_NOT_INITIALIZED;

  bool init(const System::ConfigMap* config);
  void finalize();

  EntityData load_mesh_vertex_buffer(const char* obj_file_path);

  EntityData projectile_data;

  EcsId create_player_entity(const EntityData* entity_data);
  EcsId create_asteroid_entity(const EntityData* entity_data, float world_half_edge);
  EcsId create_projectile_entity(const PhysicsComponent* player_physics);
};

} //namespace
} //namespace
