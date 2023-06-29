// loop.cpp
#include "loop.h"

#include "math/transform.h"

#include "debug.h"

namespace Asteroids {
namespace Game {

constexpr float MIN_VIEW_RECT_HALF_WIDTH = 1000.0F;
constexpr float MAX_VIEW_RECT_HALF_WIDTH = 100000.0F;

bool Loop::init(Global* global) {
  ASSERT(global);
  global_ = global;

  aspect_ratio_ = global->renderer.window_width() / (float)global_->renderer.window_height();
  view_rect_half_width_ = MIN_VIEW_RECT_HALF_WIDTH;
  camera_position_ = Math::V3(0, 0, 10);

  entity_tree_.init(global_->quadtree_arena, 10, Global::WORLD_HALF_EDGE);

  init_asteroids();

  running_ = true;
  return running_;
}

void Loop::init_asteroids() {
  //ASSERT(global_);

  //auto physics_component = &global_->entity_list.physics_components[1];
  //auto physics_component_end = global_->entity_list.physics_components + global_->entity_list.physics_components_used;

  //System::StopWatch timer;
  //while (physics_component < physics_component_end) {
  //  entity_tree_.insert(physics_component->entity_id, physics_component->aabb);
  //  physics_component++;
  //}
  //System::log_info("QT: %lf", timer.elapsed_ms());
}

void Loop::finalize() {

}

void Loop::run() {

  uint64_t previous_frame_time = 0;
  uint64_t delta_time = 0;
  System::StopWatch timer;

  while (running_) {
    SDL_PumpEvents();
    global_->input.update();

    //FIXME: Switch to menu loop instead.
    if (global_->input.quit_pressed()) {
      running_ = false;
    }

    if (global_->input.zoom_in_pressed()) {
      view_rect_half_width_ = fmaxf(MIN_VIEW_RECT_HALF_WIDTH, view_rect_half_width_ - 10.0F);
    }

    if (global_->input.zoom_out_pressed()) {
      view_rect_half_width_ = fminf(MAX_VIEW_RECT_HALF_WIDTH, view_rect_half_width_ + 10.0F);
    }


    delta_time = SDL_GetTicks() - previous_frame_time;
    previous_frame_time = SDL_GetTicks();

    timer.reset();
    update(delta_time);
    System::log_info("UPDATE: %lf", timer.elapsed_ms());
    timer.reset();
    render();
    System::log_info("RENDER: %lf", timer.elapsed_ms());
  }
}

void Loop::update(float delta_time) {
  
  const auto player_position = update_player_entity(&global_->entity_list.entities[0], delta_time);

  Entity* none_player_entity = &global_->entity_list.entities[1];
  Entity* entities_end = global_->entity_list.entities + global_->entity_list.entities_used;
  
  entity_tree_.finalize();
  entity_tree_.init(global_->quadtree_arena, 10, Global::WORLD_HALF_EDGE);

  for (; none_player_entity < entities_end; none_player_entity++) {
    update_entity(none_player_entity, delta_time);
  }

  update_view_projection(player_position);
}

void Loop::update_view_projection(const Math::V3& player_position) {
  camera_position_.x = player_position.x;
  camera_position_.y = player_position.y;

  view_matrix_ = Math::look_at(camera_position_, player_position, Math::V3{0, 1, 0});

  const float right = view_rect_half_width_ * aspect_ratio_;
  projection_matrix_ = Math::orthographic(-right, right, -view_rect_half_width_, view_rect_half_width_, 0.1F, -10000.0F);

  //projection_matrix_ = Math::perspective(90, aspect_ratio_, 0.1F, -10000.0F);
}

void Loop::update_entity(const Entity* entity, float delta_time) {

  auto physics_component = &global_->entity_list.physics_components[entity->physics_component_idx];
  auto render_component = &global_->entity_list.render_components[entity->render_component_idx];

  physics_component->aabb.pos = physics_component->aabb.pos + (physics_component->velocity * delta_time);
  render_component->world_transform = Math::translate(physics_component->aabb.pos);

  entity_tree_.insert(physics_component->entity_id, physics_component->aabb);
}

Math::V3 Loop::update_player_entity(const Entity* player_entity, float delta_time) {
  constexpr float rotation = Math::RADIANS(0.5F);
  constexpr float scale = 15.0F;

  auto input = &global_->input;
  bool player_moved = false;

  auto player_physics = &global_->entity_list.physics_components[player_entity->physics_component_idx];
  auto player_render = &global_->entity_list.render_components[player_entity->render_component_idx];
  auto player_sound = &global_->entity_list.sound_components[player_entity->sound_component_idx];

  player_physics->aabb.half_edge = scale;

  if (input->rotate_right_pressed()) {
    player_physics->orientation -= (rotation * delta_time);
  }

  if (input->rotate_left_pressed()) {
    player_physics->orientation += (rotation * delta_time);
  }

  if (input->impulse_pressed()) {
    auto velocity = Math::xyz(Math::rotate_z_axis(player_physics->orientation) * Math::xyzw(player_physics->velocity));
    player_physics->aabb.pos = player_physics->aabb.pos + (velocity * delta_time);
    if (!input->impulse_was_pressed()) {
      global_->sound_player.play_sound(player_sound->sound_indecies[0]);
    }
    player_moved = true;
  }

  if (input->shoot_pressed() && !input->shoot_was_pressed()) {
    //Entity* projectile = global_->entity_list.create_entity(PHYSICS_COMPONENT | RENDER_COMPONENT);
    global_->create_projectile_entity(player_physics);
  }

  const auto position = player_physics->aabb.pos;
  player_render->world_transform = Math::scale(scale, scale, scale) * Math::rotate_z_axis(-player_physics->orientation) * Math::translate(position);

  QTNode* containing_node = entity_tree_.query(player_physics->aabb);
  ASSERT(containing_node);

  /*PhysicsComponent* asteroid_physics_component = &global_->entity_list.physics_components[1];
  PhysicsComponent* asteroid_physics_component_end = global_->entity_list.physics_components + global_->entity_list.physics_components_used;

  while (asteroid_physics_component < asteroid_physics_component_end) {
    if (player_moved && player_physics_component->aabb.test_aabb(asteroid_physics_component->aabb)) {
      global_->sound_player.play_sound(player_sound_component->sound_indecies[1]);
    }

    asteroid_physics_component++;
  }*/

  return position;
}

void Loop::find_colliding_entities(QTNode* node) {
  
}

void Loop::render() {
  auto renderer = &global_->renderer;
  RenderComponent* render_component = global_->entity_list.render_components;
  RenderComponent* render_component_end = render_component + global_->entity_list.render_components_used;

  const auto main_shader_handle = global_->main_shader_handle;

  renderer->begin_frame();
  renderer->use_shader_program(main_shader_handle);
  renderer->shader_set_uniform(renderer->shader_uniform_location(main_shader_handle, "V"), view_matrix_);
  renderer->shader_set_uniform(renderer->shader_uniform_location(main_shader_handle, "P"), projection_matrix_);

  while (render_component < render_component_end) {

    /* renderer->shader_set_uniform(renderer->shader_uniform_location(main_shader_handle, "M"), planet_world_matrix_);
     renderer->shader_set_uniform(renderer->shader_uniform_location(main_shader_handle, "V"), view_matrix_);
     renderer->shader_set_uniform(renderer->shader_uniform_location(main_shader_handle, "P"), background_projection_matrix_);
     renderer->render_vertex_array(planet_va_index);*/

    renderer->shader_set_uniform(renderer->shader_uniform_location(main_shader_handle, "M"), render_component->world_transform);
    renderer->render_vertex_array(render_component->vertex_array_idx);

    render_component++;
  }

  //FIXME:
 /* renderer->shader_set_uniform(renderer->shader_uniform_location(main_shader_handle, "M"), Math::M4_identity());
  PhysicsComponent* physics_component = global_->entity_list.physics_components;
  PhysicsComponent* physics_component_end = physics_component + global_->entity_list.physics_components_used;

  while (physics_component < physics_component_end) {
    renderer->render_aabb(physics_component->aabb);
    physics_component++;
  }
  
  Debug::render_quadtree(&entity_tree_);
  */
  renderer->end_frame();
}

} //namespace
} //namespace
