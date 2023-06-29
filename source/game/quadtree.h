// quadtree.h
#pragma once

#include "system/memory.h"

#include "math/aabb.h"

#include "rendering/renderer.h"

#include "ecs.h"

namespace Asteroids {
namespace Game {

struct EcsIdNode {
  EcsId id;
  EcsIdNode* next;
};

struct QTNode {
  Math::AABB aabb;
  EcsIdNode* entity_list;

  QTNode* nw_child;
  QTNode* ne_child;
  QTNode* sw_child;
  QTNode* se_child;
};

class QuadTree final {
  DISABLE_COPY_AND_MOVE(QuadTree);
public:
  QuadTree() = default;
  ~QuadTree() = default;

  bool init(System::MemoryArena* arena, int32_t max_depth, float max_half_edge);
  void finalize();

  bool insert(EcsId entity_id, const Math::AABB& aabb);
  QTNode* query(const Math::AABB& aabb);

  bool subdivide(QTNode* node, int32_t depth);
  QTNode* try_subdivide(QTNode* node, const Math::AABB& aabb, int32_t depth);

  QTNode* find_containing_node(QTNode* node, const Math::AABB& aabb);

  QTNode* root_ = nullptr;
  int32_t max_depth_ = 0;

  System::MemoryArena* arena_ = nullptr;
};

} //namespace
} //namespace
