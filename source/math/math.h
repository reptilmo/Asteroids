// math.h
#pragma once

#include "system/system.h"

namespace Asteroids {
namespace Math {

constexpr float PI = static_cast<float>(3.1415926535);
constexpr float HALF_PI = PI * 0.5F;

constexpr float RADIANS(float degrees) { 
  return degrees * PI / 180.0F;
}

inline float sin(float radians) {
  return sinf(radians);
}

inline float cos(float radians) {
  return cosf(radians);
}

inline float tan(float radians) {
  return tanf(radians);
}

inline float rsqrt(float f) {
  __m128 reg = _mm_set1_ps(f);
  return _mm_cvtss_f32(_mm_rsqrt_ss(reg));
}

inline float abs(float f) {
  return fabs(f);
}

} //namespace
} //namespace
