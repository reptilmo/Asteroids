// system.h
#pragma once
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include <SDL.h>

#ifndef RELEASE
#define ASSERT(condition)                                                                              \
  do {                                                                                                 \
    if (!(condition)) {                                                                                \
      abort();                                                                                         \
    }                                                                                                  \
  } while (0) // NOTE: No semicolon
#else
#define ASSERT(condition) (condition)
#endif

constexpr size_t MAX_SIZE_T = size_t(-1);

#define DISABLE_COPY_AND_MOVE(ClassName)                                                               \
  ClassName(const ClassName&) = delete;                                                                \
  ClassName& operator= (const ClassName&) = delete;                                                    \
  ClassName(ClassName&&) noexcept = delete;                                                            \
  ClassName& operator= (ClassName&&) noexcept = delete

namespace Asteroids {
namespace System {

template <typename T> T min(T left, T right) {
  return left < right ? left : right;
}

template <typename T> T max(T left, T right) {
  return left > right ? left : right;
}

template <typename... Args> void log_error(const char* fmt, Args... args) {
  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, fmt, args...);
}
template <typename... Args> void log_info(const char* fmt, Args... args) {
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, fmt, args...);
}
template <typename... Args> void log_debug(const char* fmt, Args... args) {
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, fmt, args...);
}


class StopWatch final {
  DISABLE_COPY_AND_MOVE(StopWatch);
public:
  StopWatch() : last_(SDL_GetPerformanceCounter()) {}
  ~StopWatch() = default;

  double elapsed_ms() {
    return ((SDL_GetPerformanceCounter() - last_) * 1000.0) / (double)SDL_GetPerformanceFrequency();
  }

  void reset() {
    last_ = SDL_GetPerformanceCounter();
  }
private:
  uint64_t last_;
};

} //namespace
} //namespace
