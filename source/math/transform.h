// transform.h
#pragma once

#include "matrix4.h"

namespace Asteroids {
namespace Math {

inline M4 translate(const V3& p) {
  return M4{{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    p.x, p.y, p.z, 1,
  }};
}

inline M4 rotate_x_axis(float radians) {
  const float c = Math::cos(radians);
  const float s = Math::sin(radians);

  return M4{{
    1, 0, 0, 0,
    0, c, -s, 0,
    0, s, c, 0,
    0, 0, 0, 1,
  }};
}

inline M4 rotate_y_axis(float radians) {
  const float c = Math::cos(radians);
  const float s = Math::sin(radians);

  return M4{{
    c, 0, s, 0,
    0, 1, 0, 0,
    -s, 0, c, 0,
    0, 0, 0, 1,
  }};
}

inline M4 rotate_z_axis(float radians) {
  const float c = Math::cos(radians);
  const float s = Math::sin(radians);

  return M4{{
    c, -s, 0, 0,
    s, c, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
  }};
}

inline M4 scale(float x, float y, float z) {
  return M4{{
    x, 0, 0, 0,
    0, y, 0, 0,
    0, 0, z, 0,
    0, 0, 0, 1,
  }};
}

inline M4 orthographic(float left, float right, float bottom, float top,  float near, float far) {

  const float h_factor = 1.0F / (right -  left);
  const float v_factor = 1.0F / (top - bottom);
  const float d_factor = 1.0F / (far - near);

  return M4{{
    2.0F * h_factor,
    0,
    0,
    0,

    0,
    2.0F * v_factor,
    0,
    0,

    0,
    0,
    -2.0F * d_factor,
    0,

    -(right + left) * h_factor,
    -(top + bottom) * v_factor,
    -(far + near) * d_factor,
    1,
  }};
}

inline M4 perspective(float fov, float aspect_ration, float front, float back) {

  const float top = front * Math::tan(RADIANS(fov * 0.5F));
  const float bottom = -top;
  const float right = top * aspect_ration;
  const float left = -right;

  return M4{{
    2 * front / (right - left),
    0,
    0,
    0,

    0,
    2 * front / (top - bottom),
    0,
    0,

    (right + left) / (right - left),
    (top + bottom) / (top - bottom),
    -(back + front) / (back - front),
    -1,

    0,
    0,
    -2 * front * back / (back - front),
    0,
  }};
}

inline M4 look_at(const V3& eye, const V3& target, const V3& up) {
  const V3 z_axis = normalized(target - eye);
  const V3 x_axis = normalized(cross(z_axis, up));
  const V3 y_axis = cross(x_axis, z_axis);

  return M4{{
    x_axis.x, y_axis.x, z_axis.x, 0,
    x_axis.y, y_axis.y, z_axis.y, 0,
    x_axis.z, y_axis.z, z_axis.z, 0,
    -dot(x_axis, eye), -dot(y_axis, eye), -dot(z_axis, eye), 1
  }};
}

}//namespace
}//namespace
