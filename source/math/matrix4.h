// matrix4.h
#pragma once

#include "vector4.h"

namespace Asteroids {
namespace Math {
// Column major 4 by 4 matrix

struct M4 {
  float m[16];
};

inline M4 M4_identity() {
  return M4{{
    1.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 1.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 1.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 1.0F,
  }};
}

inline M4 M4_new(float* f) {
  return M4{{
    f[0], f[1], f[2], f[3],
    f[4], f[5], f[6], f[7],
    f[8], f[9], f[10], f[11],
    f[12], f[13], f[14], f[15],
  }};
}

//FIXME: Implement SIMD versions
inline V4 operator*(const M4& m, const V4& v) noexcept {
  return V4{
    m.m[0] * v.v[0] + m.m[1] * v.v[1] + m.m[2] * v.v[2] + m.m[3] * v.v[3],
    m.m[4] * v.v[0] + m.m[5] * v.v[1] + m.m[6] * v.v[2] + m.m[7] * v.v[3],
    m.m[8] * v.v[0] + m.m[9] * v.v[1] + m.m[10] * v.v[2] + m.m[11] * v.v[3],
    m.m[12] * v.v[0] + m.m[13] * v.v[1] + m.m[14] * v.v[2] + m.m[15] * v.v[3],
  };
}

inline M4 operator*(const M4& l, const M4& r) noexcept {
  return M4{{
    l.m[0] * r.m[0] + l.m[1] * r.m[4] + l.m[2] * r.m[8] + l.m[3] * r.m[12],
    l.m[0] * r.m[1] + l.m[1] * r.m[5] + l.m[2] * r.m[9] + l.m[3] * r.m[13],
    l.m[0] * r.m[2] + l.m[1] * r.m[6] + l.m[2] * r.m[10] + l.m[3] * r.m[14],
    l.m[0] * r.m[3] + l.m[1] * r.m[7] + l.m[2] * r.m[11] + l.m[3] * r.m[15],

    l.m[4] * r.m[0] + l.m[5] * r.m[4] + l.m[6] * r.m[8] + l.m[7] * r.m[12],
    l.m[4] * r.m[1] + l.m[5] * r.m[5] + l.m[6] * r.m[9] + l.m[7] * r.m[13],
    l.m[4] * r.m[2] + l.m[5] * r.m[6] + l.m[6] * r.m[10] + l.m[7] * r.m[14],
    l.m[4] * r.m[3] + l.m[5] * r.m[7] + l.m[6] * r.m[11] + l.m[7] * r.m[15],

    l.m[8] * r.m[0] + l.m[9] * r.m[4] + l.m[10] * r.m[8] + l.m[11] * r.m[12],
    l.m[8] * r.m[1] + l.m[9] * r.m[5] + l.m[10] * r.m[9] + l.m[11] * r.m[13],
    l.m[8] * r.m[2] + l.m[9] * r.m[6] + l.m[10] * r.m[10] + l.m[11] * r.m[14],
    l.m[8] * r.m[3] + l.m[9] * r.m[7] + l.m[10] * r.m[11] + l.m[11] * r.m[15],

    l.m[12] * r.m[0] + l.m[13] * r.m[4] + l.m[14] * r.m[8] + l.m[15] * r.m[12],
    l.m[12] * r.m[1] + l.m[13] * r.m[5] + l.m[14] * r.m[9] + l.m[15] * r.m[13],
    l.m[12] * r.m[2] + l.m[13] * r.m[6] + l.m[14] * r.m[10] + l.m[15] * r.m[14],
    l.m[12] * r.m[3] + l.m[13] * r.m[7] + l.m[14] * r.m[11] + l.m[15] * r.m[15],
  }};
}

} //namespace
} //namespace
