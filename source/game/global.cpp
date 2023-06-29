// global.cpp
#include "global.h"

#include "system/random.h"
#include "math/transform.h"

namespace Asteroids {
namespace Game {

const size_t Global::FILE_IO_ARENA_SIZE = System::MB(10);
const size_t Global::MESH_ARENA_SIZE = System::MB(10);
const size_t Global::RENDERER_ARENA_SIZE = System::MB(10);
const size_t Global::ENTITY_ARENA_SIZE = System::MB(10);
const size_t Global::QUADTREE_ARENA_SIZE = System::MB(16);

const size_t Global::MAX_MESH_COUNT = 10;
const size_t Global::MAX_VERTEX_ARRAY_COUNT = 10;
const size_t Global::MAX_TEXTURE_COUNT = 10;
const size_t Global::MAX_ENTITY_COUNT = 10000;

const float Global::WORLD_HALF_EDGE = 100000.0F;

Global::Global() {
  file_io_arena = System::memory_arena_create("FILEIO", FILE_IO_ARENA_SIZE);
  mesh_arena = System::memory_arena_create("MESH", MESH_ARENA_SIZE);
  renderer_arena = System::memory_arena_create("RENDER", RENDERER_ARENA_SIZE);
  entity_arena = System::memory_arena_create("ENTITY", ENTITY_ARENA_SIZE);
  quadtree_arena = System::memory_arena_create("QUADTREE", QUADTREE_ARENA_SIZE);
}

Global::~Global() {
  System::memory_arena_free(file_io_arena);
  System::memory_arena_free(mesh_arena);
  System::memory_arena_free(renderer_arena);
  System::memory_arena_free(entity_arena);
}

void Global::finalize() {
  renderer.finalize();
  mesh_builder.finalize();
  sound_player.finalize();
  entity_list.finalize();
}

bool Global::init(const System::ConfigMap* config) {
  ASSERT(config);
  input.init(config);

  if (!renderer.init(renderer_arena)) {
    return false;
  }

  const bool fullscreen = config->value_int("fullscreen", 0) > 0 ? true : false;
  const int window_width = config->value_int("window_width", 1024);
  const int window_height = config->value_int("window_height", 768);

  if (!renderer.init_window(window_width, window_height, fullscreen)) {
    return false;
  }

  if (!renderer.init_opengl()) {
    return false;
  }

  if (!renderer.init_vertex_array_list(MAX_VERTEX_ARRAY_COUNT)) {
    return false;
  }

  if (!sound_player.init(-1)) {
    return false;
  }

  System::FileIO asset_file_io;
  if (!asset_file_io.init(file_io_arena)) {
    return false;
  }

  //FIXME: Load shaders and assets from 'resource pack'.
  auto main_vert_shader_buffer = asset_file_io.read_bytes("E://Asteroids//shaders//test.vert", System::KB(10));
  if (!is_valid(&main_vert_shader_buffer)) {
    return false;
  }

  auto main_frag_shader_buffer = asset_file_io.read_bytes("E://Asteroids//shaders//test.frag", System::KB(10));
  if (!is_valid(&main_frag_shader_buffer)) {
    return false;
  }

  main_shader_handle = renderer.build_shader_program(&main_vert_shader_buffer, &main_frag_shader_buffer);
  if (main_shader_handle == 0) {
    return false;
  }

  if (!mesh_builder.init(mesh_arena, MAX_MESH_COUNT)) {
    return false;
  }
  
  if (!entity_list.init(entity_arena, MAX_ENTITY_COUNT)) {
    return false;
  }

  EntityData player = load_mesh_vertex_buffer("E://Asteroids-resources//ship-2.obj");
  player.sound_indecies[0] = sound_player.load_wav("E://Asteroids-resources//ship-propultion.wav");
  player_entity_id = create_player_entity(&player);

  EntityData asteroid = load_mesh_vertex_buffer("E://Asteroids-resources//asteroid-mesh.obj");
  asteroid.sound_indecies[0] = sound_player.load_wav("E://Asteroids-resources//asteroid-explosion.wav");
  for (int i = 0; i < 5000; i++) {
    create_asteroid_entity(&asteroid, WORLD_HALF_EDGE);
  }

  projectile_data = load_mesh_vertex_buffer("E://Asteroids-resources//projectile.obj");

  System::memory_arena_free(file_io_arena);

  if (sound_player.load_mp3("E://Asteroids-resources//music.mp3")) {
    sound_player.play_music();
  }

  return true;
}

EntityData Global::load_mesh_vertex_buffer(const char* obj_file_path) {
  EntityData out{
    .mesh = nullptr, 
    .vertex_array_idx = -1,
    .sound_indecies = {},
  };

  System::FileIO io;
  if (!io.init(file_io_arena)) {
    return out;
  }

  auto mesh_buffer = io.read_bytes(obj_file_path, System::KB(100));
  if (!is_valid(&mesh_buffer)) {
    return out;
  }

  const auto mesh = mesh_builder.mesh_from_obj(&mesh_buffer, 100, 100);
  if (!mesh) {
    return out;
  }

  const size_t va_idx = renderer.build_vertex_array(mesh);
  if (va_idx == MAX_SIZE_T) {
    return out;
  }

  out.mesh = mesh;
  out.vertex_array_idx = va_idx;
  
  return out;
}

EcsId Global::create_player_entity(const EntityData* data) {
  ASSERT(entity_list.entities);
  Entity* player = entity_list.create_entity(PHYSICS_COMPONENT | SOUND_COMPONENT | RENDER_COMPONENT);

  entity_list.render_components[player->render_component_idx].vertex_array_idx = data->vertex_array_idx;
  entity_list.sound_components[player->sound_component_idx].sound_indecies[0] = data->sound_indecies[0];

  entity_list.physics_components[player->physics_component_idx].orientation = 0.0F;
  entity_list.physics_components[player->physics_component_idx].velocity = {0.0F, 0.5F, 0.0F};
  entity_list.physics_components[player->physics_component_idx].aabb.pos = Math::V3{0.0F, 0.0F, 0.0F};

  const size_t mesh_vertex_count = data->mesh->vertex_count;
  Math::AABB* aabb = &entity_list.physics_components[player->physics_component_idx].aabb;

  for (size_t i = 0; i < mesh_vertex_count; i++) {
    aabb->update_edge(data->mesh->vertices[i].position);
  }

  return player->entity_id;
}

EcsId Global::create_asteroid_entity(const EntityData* data, float world_half_edge) {
  ASSERT(entity_list.entities);
  static System::Random r;

  Entity* asteroid = entity_list.create_entity(PHYSICS_COMPONENT | RENDER_COMPONENT | SOUND_COMPONENT);
  entity_list.render_components[asteroid->render_component_idx].vertex_array_idx = data->vertex_array_idx;
  entity_list.sound_components[asteroid->sound_component_idx].sound_indecies[0] = data->sound_indecies[0];

  entity_list.physics_components[asteroid->physics_component_idx].orientation = 0.0F;
  entity_list.physics_components[asteroid->physics_component_idx].velocity = {0.0F, 0.0F, 0.0F};
  entity_list.physics_components[asteroid->physics_component_idx].aabb.pos = Math::V3{
    r.random_float(-2, 2) * world_half_edge,
    r.random_float(-2, 2) * world_half_edge,
    0.0F
  };

  entity_list.sound_components[asteroid->sound_component_idx].sound_indecies[0] = data->sound_indecies[0];

  const size_t mesh_vertex_count = data->mesh->vertex_count;
  Math::AABB* aabb = &entity_list.physics_components[asteroid->physics_component_idx].aabb;

  for (size_t i = 0; i < mesh_vertex_count; i++) {
    aabb->update_edge(data->mesh->vertices[i].position);
  }

  return asteroid->entity_id;
}

EcsId Global::create_projectile_entity(const PhysicsComponent* player_physics) {
  ASSERT(entity_list.entities);
  
  Entity* projectile = entity_list.create_entity(PHYSICS_COMPONENT | RENDER_COMPONENT);
  entity_list.render_components[projectile->render_component_idx].vertex_array_idx = projectile_data.vertex_array_idx;

  entity_list.physics_components[projectile->physics_component_idx].orientation = player_physics->orientation;
  entity_list.physics_components[projectile->physics_component_idx].velocity =
    Math::xyz(Math::rotate_z_axis(player_physics->orientation) * Math::xyzw(player_physics->velocity * 3.0F)); //FIXME:

  entity_list.physics_components[projectile->physics_component_idx].aabb.pos = player_physics->aabb.pos;

  const size_t mesh_vertex_count = projectile_data.mesh->vertex_count;
  Math::AABB* aabb = &entity_list.physics_components[projectile->physics_component_idx].aabb;

  for (size_t i = 0; i < mesh_vertex_count; i++) {
    aabb->update_edge(projectile_data.mesh->vertices[i].position);
  }

  return projectile->entity_id;
}

} //namespace
} //namespace
