// loop.h
#pragma once

#include "global.h"
#include "quadtree.h"

#include "math/matrix4.h"

namespace Asteroids {
namespace Game {

constexpr int32_t MAX_COLLIDING_ENTITIES = 15;

class Loop final {
  DISABLE_COPY_AND_MOVE(Loop);
public:
  Loop() = default;
  ~Loop() = default;

  bool init(Global* global);
  void finalize();

  void run();

private:
  void init_asteroids();

  void update(float delta_time);

  Math::V3 update_player_entity(const Entity* entity, float delta_time);
  void update_entity(const Entity* entity, float delat_time);
  void update_view_projection(const Math::V3& player_position);

  void render();

  void find_colliding_entities(QTNode* node);

private:
  Global* global_ = nullptr;
  bool running_ = false;

  float aspect_ratio_;

  float view_rect_half_width_;

  Math::V3 camera_position_;

  Math::M4 view_matrix_;
  Math::M4 projection_matrix_;
  Math::M4 background_projection_matrix_;

  QuadTree entity_tree_;
};

} //namespace
} //namespace
