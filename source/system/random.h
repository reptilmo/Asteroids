// random.h
#pragma once

#include "system.h"

namespace Asteroids {
namespace System {

class Random final {
  DISABLE_COPY_AND_MOVE(Random);
public:
  Random();
  ~Random() = default;

  float random_float(float low, float high);
};

} //namespace
} //namespace
