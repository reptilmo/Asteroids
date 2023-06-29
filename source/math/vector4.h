// vector4.h
#pragma once

#include "math.h"
#include "vector3.h"

namespace Asteroids {
namespace Math {

// 4D column vector
struct V4 final {
  V4() : v{ 0.0F, 0.0F, 0.0F, 0.0F, } {}
  V4(float x, float y, float z, float w) : v{ x, y, z, w, } {}
  explicit V4(float* f) : v{ f[0], f[1], f[2], f[3], } {}
  ~V4() = default;

  float v[4];
};

inline V3 xyz(const V4& v) { return V3{v.v[0], v.v[1], v.v[2]}; }
inline V4 xyzw(const V3& v) { return V4{v.x, v.y, v.z, 1.0F}; }

} //namespace
} //namespace
