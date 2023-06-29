// math.h
#pragma once

#include "math.h"

namespace Asteroids {
namespace Math {
// 3D column vector
struct V3 final {
  V3() : x(0.0F), y(0.0F), z(0.0F) {}
  V3(float a, float b, float c) : x(a), y(b), z(c) {}
  explicit V3(float* f) : x(f[0]), y(f[1]), z(f[2]) {}
  ~V3() = default;

  float x, y, z;
};

inline V3 operator*(const V3& v, float s) {
  return V3{
    v.x * s,
    v.y * s,
    v.z * s,
  }; 
}

inline V3 operator*(float s, const V3& v) {
  return V3{
    v.x * s,
    v.y * s,
    v.z * s,
  };
}

inline V3 operator+(const V3& a, const V3 b) {
  return V3{
    a.x + b.x,
    a.y + b.y,
    a.z + b.z,
  };
}

inline V3 operator-(const V3& a, const V3 b) {
  return V3{
    a.x - b.x,
    a.y - b.y,
    a.z - b.z,
  };
}

inline float dot(const V3& a, const V3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline void normalize(V3& v) {
  const float r = Math::rsqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  v.x *= r;
  v.y *= r;
  v.z *= r;
}

inline V3 normalized(const V3& v) {
  const float r = Math::rsqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  return V3{
    v.x * r,
    v.y * r,
    v.z * r,
  };
}

inline V3 cross(const V3& a, const V3& b) {
  return V3{
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x,
  };
}

} //namespace
} //namespace
