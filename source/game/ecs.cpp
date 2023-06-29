// ecs.cpp
#include "ecs.h"


namespace Asteroids {
namespace Game {

bool EntityComponentList::init(System::MemoryArena* arena, int32_t max_entities) {
  ASSERT(arena && arena->allocated_size > 0);
  if (!arena || !arena->allocated_size) {
    return false;
  }

  entity_arena = arena;
  max_entities_ = max_entities;

  entities = (Entity*)System::memory_arena_alloc(entity_arena, max_entities_, sizeof(Entity));
  ASSERT(entities);
  if (!entities) {
    return false;
  }

  render_components = (RenderComponent*)System::memory_arena_alloc(entity_arena, max_entities_, sizeof(RenderComponent));
  ASSERT(render_components);
  if (!render_components) {
    return false;
  }

  physics_components = (PhysicsComponent*)System::memory_arena_alloc(entity_arena, max_entities_, sizeof(PhysicsComponent));
  ASSERT(physics_components);
  if (!physics_components) {
    return false;
  }

  sound_components = (SoundComponent*)System::memory_arena_alloc(entity_arena, max_entities_, sizeof(SoundComponent));
  ASSERT(sound_components);
  if (!sound_components) {
    return false;
  }

  return true;
}

void EntityComponentList::finalize() {

}

Entity* EntityComponentList::create_entity(int components) {
  ASSERT(entities_used < max_entities_);

  const int32_t new_entity_idx = entities_used++;

  Entity* entity = &entities[new_entity_idx];
  entity->entity_id = new_entity_idx;
  entity->physics_component_idx = ECSID_NOT_INITIALIZED;
  entity->render_component_idx = ECSID_NOT_INITIALIZED;
  entity->sound_component_idx = ECSID_NOT_INITIALIZED;

  if (components & RENDER_COMPONENT) {
    const int32_t render_component_idx = render_components_used++;
    entity->render_component_idx = render_component_idx;
    render_components[render_component_idx].entity_id = new_entity_idx;
  }

  if (components & PHYSICS_COMPONENT) {
    const int32_t physics_component_idx = physics_components_used++;
    entity->physics_component_idx = physics_component_idx;
    physics_components[physics_component_idx].entity_id = new_entity_idx;
  }

  if (components & SOUND_COMPONENT) {
    const int32_t sound_component_idx = sound_components_used++;
    entity->sound_component_idx = sound_component_idx;
    sound_components[sound_component_idx].entity_id = new_entity_idx;
  }

  return entity;
}

} //namespace
} //namespace
