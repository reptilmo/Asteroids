// ecs.h
#pragma once

#include "system/system.h"
#include "system/memory.h"
#include "math/vector3.h"
#include "math/matrix4.h"
#include "math/aabb.h"

namespace Asteroids {
namespace Game {

using EcsId = int32_t;
constexpr EcsId ECSID_NOT_INITIALIZED = -1;
constexpr int32_t MAX_SOUND_COMPONENT_SOUNDS = 4;

enum EntityComponentTypes {
  LIFETIME_COMPONENT = 1,
  PHYSICS_COMPONENT = 2,
  RENDER_COMPONENT = 3,
  SOUND_COMPONENT = 4,
};

struct Entity {
  EcsId entity_id;
  EcsId render_component_idx;
  EcsId physics_component_idx;
  EcsId sound_component_idx;
  EcsId lifetime_component_idx;
  bool defunct;
};

struct RenderComponent {
  Math::M4 world_transform;
  int32_t vertex_array_idx;
  int32_t texture_idx;
  EcsId entity_id;
};

struct PhysicsComponent {
  Math::AABB aabb;
  Math::V3 velocity;
  Math::V3 acceleration;
  float orientation; // In XY plane (around Z axis)
  float mass;
  EcsId entity_id;
};

struct SoundComponent {
  int32_t sound_indecies[MAX_SOUND_COMPONENT_SOUNDS];
  EcsId entity_id;
};

struct LifetimeComponent {
  int64_t lifetime_ms;
  EcsId entity_id;
};

class EntityComponentList final {
  DISABLE_COPY_AND_MOVE(EntityComponentList);
public:
  EntityComponentList() = default;
  ~EntityComponentList() = default;

  bool init(System::MemoryArena* arena, int32_t max_entity_count);
  void finalize();

  Entity* create_entity(int components);

public:
  Entity* entities = nullptr;
  int32_t entities_used = 0;

  LifetimeComponent* lifetime_components = nullptr;
  int32_t lifetime_componens_used = 0;

  PhysicsComponent* physics_components = nullptr;
  int32_t physics_components_used = 0;
  
  RenderComponent* render_components = nullptr;
  int32_t render_components_used = 0;

  SoundComponent* sound_components = nullptr;
  int32_t sound_components_used = 0;

private:
  int32_t max_entities_ = 0;
  System::MemoryArena* entity_arena = nullptr;
};

} //namespace
} //namespace
