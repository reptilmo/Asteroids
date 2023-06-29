// aabb.h
#pragma once

#include "vector3.h"

namespace Asteroids {
namespace Math {

struct AABB final {
  AABB() = default;
  ~AABB() = default;
  AABB(const V3& c, float e) : pos(c), half_edge(e) {}
  
  void update_edge(const V3& p) {
    half_edge = fmaxf(half_edge, fmaxf(p.z, fmaxf(p.y, p.x)));
  }

  bool point_inside(const V3& in) const {
    return (pos.x + half_edge) >= in.x
      && (pos.x - half_edge) <= in.x
      && (pos.y + half_edge) >= in.y
      && (pos.y - half_edge) <= in.y
      && (pos.z + half_edge) >= in.z
      && (pos.z - half_edge) <= in.z;
  }

  bool intersects(const AABB& in) const {
    return (pos.x + half_edge) >= (in.pos.x - in.half_edge)
      && (pos.x - half_edge) <= (in.pos.x + in.half_edge)
      && (pos.y + half_edge) >= (in.pos.y - in.half_edge)
      && (pos.y - half_edge) <= (in.pos.y + in.half_edge)
      && (pos.z + half_edge) >= (in.pos.z - in.half_edge)
      && (pos.z - half_edge) <= (in.pos.z + in.half_edge);
  }

  bool intersects_xy(const AABB& in) const {
    return (pos.x + half_edge) >= (in.pos.x - in.half_edge)
      && (pos.x - half_edge) <= (in.pos.x + in.half_edge)
      && (pos.y + half_edge) >= (in.pos.y - in.half_edge)
      && (pos.y - half_edge) <= (in.pos.y + in.half_edge);
  }

  bool contains_xy(const AABB& in) const {
    return (pos.x - half_edge) <= (in.pos.x - in.half_edge)
      && (pos.x + half_edge) >= (in.pos.x + in.half_edge)
      && (pos.y - half_edge) <= (in.pos.y - in.half_edge)
      && (pos.y + half_edge) >= (in.pos.y + in.half_edge);
  }

  V3 pos;
  float half_edge = 1.0F;
};

} //namespace
} //namespace
