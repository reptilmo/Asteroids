// random.cpp
#include "random.h"

namespace Asteroids {
namespace System {


Random::Random() {
#ifdef RELEASE
  srand(time(nullptr));
#else
  srand(1137);
#endif
}

float Random::random_float(float low, float high) {
  return low + rand() / (float(RAND_MAX) / (high - low));
}

} //namespace
} //namespace
