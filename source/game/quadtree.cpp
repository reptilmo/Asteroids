// quadtree.cpp
#include "quadtree.h"

namespace Asteroids {
namespace Game {
  
bool QuadTree::init(System::MemoryArena* arena, int32_t max_depth, float max_half_edge) {
  ASSERT(arena && arena->allocated_size > sizeof(QTNode) && max_depth >= 1);
  arena_ = arena;

  root_ = (QTNode*)System::memory_arena_alloc(arena_, 1, sizeof(QTNode));
  if (!root_) {
    return false;
  }

  root_->aabb.pos.x = 0.0F;
  root_->aabb.pos.y = 0.0F;
  root_->aabb.pos.z = 0.0F;
  root_->aabb.half_edge = max_half_edge;

  max_depth_ = max_depth;
  return true;
}

void QuadTree::finalize() {
  System::memory_arena_reset(arena_);
}

bool QuadTree::insert(EcsId entity_id, const Math::AABB& aabb) {

  if (root_->aabb.contains_xy(aabb)) {
    QTNode* node = try_subdivide(root_, aabb, 1);
    if (node) {

      if (node->entity_list) {
        auto last = node->entity_list;
        while (last->next) {
          last = last->next;
        }

        last->next = (EcsIdNode*)System::memory_arena_alloc(arena_, 1, sizeof(EcsIdNode));
        if (last->next) {
          last->next->id = entity_id;
          return true;
        }
      } else {
        node->entity_list = (EcsIdNode*)System::memory_arena_alloc(arena_, 1, sizeof(EcsIdNode));
        if (node->entity_list) {
          node->entity_list->id = entity_id;
          return true;
        }
      }
    }
  }

  return false;
}

QTNode* QuadTree::query(const Math::AABB& aabb) {
  return find_containing_node(root_, aabb);
}

QTNode* QuadTree::try_subdivide(QTNode* node, const Math::AABB& aabb, int32_t depth) {

  if (depth < max_depth_) {
    Math::AABB new_aabb{ {0, 0, 0}, node->aabb.half_edge * 0.5F };

    if (node->nw_child) {
      if (node->nw_child->aabb.contains_xy(aabb)) {
        return try_subdivide(node->nw_child, aabb, depth + 1);
      }
    } else {
      new_aabb.pos.x = node->aabb.pos.x - new_aabb.half_edge;
      new_aabb.pos.y = node->aabb.pos.y + new_aabb.half_edge;
      new_aabb.pos.z = 0.0F;

      if (new_aabb.contains_xy(aabb)) {
        node->nw_child = (QTNode*)System::memory_arena_alloc(arena_, 1, sizeof(QTNode));
        if (!node->nw_child) {
          return nullptr;
        }

        node->nw_child->aabb = new_aabb;
        return try_subdivide(node->nw_child, aabb, depth + 1);
      }
    }

    if (node->ne_child) {
      if (node->ne_child->aabb.contains_xy(aabb)) {
        return try_subdivide(node->ne_child, aabb, depth + 1);
      }
    } else {
      new_aabb.pos.x = node->aabb.pos.x + new_aabb.half_edge;
      new_aabb.pos.y = node->aabb.pos.y + new_aabb.half_edge;
      new_aabb.pos.z = 0.0F;

      if (new_aabb.contains_xy(aabb)) {
        node->ne_child = (QTNode*)System::memory_arena_alloc(arena_, 1, sizeof(QTNode));
        if (!node->ne_child) {
          return nullptr;
        }

        node->ne_child->aabb = new_aabb;
        return try_subdivide(node->ne_child, aabb, depth + 1);
      }
    }

    if (node->se_child) {
      if (node->se_child->aabb.contains_xy(aabb)) {
        return try_subdivide(node->se_child, aabb, depth + 1);
      }
    } else {
      new_aabb.pos.x = node->aabb.pos.x + new_aabb.half_edge;
      new_aabb.pos.y = node->aabb.pos.y - new_aabb.half_edge;
      new_aabb.pos.z = 0.0F;

      if (new_aabb.contains_xy(aabb)) {
        node->se_child = (QTNode*)System::memory_arena_alloc(arena_, 1, sizeof(QTNode));
        if (!node->se_child) {
          return nullptr;
        }

        node->se_child->aabb = new_aabb;
        return try_subdivide(node->se_child, aabb, depth + 1);
      }
    }

    if (node->sw_child) {
      if (node->sw_child->aabb.contains_xy(aabb)) {
        return try_subdivide(node->sw_child, aabb, depth + 1);
      }
    } else {
      new_aabb.pos.x = node->aabb.pos.x - new_aabb.half_edge;
      new_aabb.pos.y = node->aabb.pos.y - new_aabb.half_edge;
      new_aabb.pos.z = 0.0F;

      if (new_aabb.contains_xy(aabb)) {
        node->sw_child = (QTNode*)System::memory_arena_alloc(arena_, 1, sizeof(QTNode));
        if (!node->sw_child) {
          return nullptr;
        }

        node->sw_child->aabb = new_aabb;
        return try_subdivide(node->sw_child, aabb, depth + 1);
      }
    }
  }

  return node;
}

bool QuadTree::subdivide(QTNode* node, int32_t depth) {

  const float child_half_edge = node->aabb.half_edge * 0.5F;

  node->nw_child = (QTNode*)System::memory_arena_alloc(arena_, 1, sizeof(QTNode));
  if (!node->nw_child) {
    return false;
  }

  node->nw_child->aabb.pos.x = node->aabb.pos.x - child_half_edge;
  node->nw_child->aabb.pos.y = node->aabb.pos.y + child_half_edge;
  node->nw_child->aabb.pos.z = 0.0F;
  node->nw_child->aabb.half_edge = child_half_edge;

  node->ne_child = (QTNode*)System::memory_arena_alloc(arena_, 1, sizeof(QTNode));
  if (!node->ne_child) {
    return false;
  }

  node->ne_child->aabb.pos.x = node->aabb.pos.x + child_half_edge;
  node->ne_child->aabb.pos.y = node->aabb.pos.y + child_half_edge;
  node->ne_child->aabb.pos.z = 0.0F;
  node->ne_child->aabb.half_edge = child_half_edge;

  node->sw_child = (QTNode*)System::memory_arena_alloc(arena_, 1, sizeof(QTNode));
  if (!node->sw_child) {
    return false;
  }

  node->sw_child->aabb.pos.x = node->aabb.pos.x - child_half_edge;
  node->sw_child->aabb.pos.y = node->aabb.pos.y - child_half_edge;
  node->sw_child->aabb.pos.z = 0.0F;
  node->sw_child->aabb.half_edge = child_half_edge;

  node->se_child = (QTNode*)System::memory_arena_alloc(arena_, 1, sizeof(QTNode));
  if (!node->se_child) {
    return false;
  }

  node->se_child->aabb.pos.x = node->aabb.pos.x + child_half_edge;
  node->se_child->aabb.pos.y = node->aabb.pos.y - child_half_edge;
  node->se_child->aabb.pos.z = 0.0F;
  node->se_child->aabb.half_edge = child_half_edge;


  if (depth > 1) {
    return subdivide(node->nw_child, depth - 1)
      && subdivide(node->ne_child, depth - 1)
      && subdivide(node->sw_child, depth - 1)
      && subdivide(node->se_child, depth - 1);
  }

  return true;
}

QTNode* QuadTree::find_containing_node(QTNode* node, const Math::AABB& aabb) {

  if (node->aabb.contains_xy(aabb)) {

    if (node->nw_child != nullptr && node->nw_child->aabb.contains_xy(aabb)) {
      return find_containing_node(node->nw_child, aabb);
    }

    if (node->ne_child != nullptr && node->ne_child->aabb.contains_xy(aabb)) {
      return find_containing_node(node->ne_child, aabb);
    }

    if (node->sw_child != nullptr && node->sw_child->aabb.contains_xy(aabb)) {
      return find_containing_node(node->sw_child, aabb);
    }

    if (node->se_child != nullptr && node->se_child->aabb.contains_xy(aabb)) {
      return find_containing_node(node->se_child, aabb);
    }

    return node;
  }

  return nullptr;
}

} //namespace
} //namespace
