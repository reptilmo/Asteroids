// memory.h
#pragma once

#include "system.h"

namespace Asteroids {
namespace System {

constexpr size_t KB(size_t bytes) { return bytes * 1024LL; }
constexpr size_t MB(size_t bytes) { return bytes * 1024LL * 1024LL; }
constexpr size_t GB(size_t bytes) { return bytes * 1024LL * 1024LL * 1024LL; }

struct MemoryArena {
  int8_t tag[8];
  size_t allocated_size;
  size_t used_size;
  uint8_t* bytes;
};

MemoryArena* memory_arena_create(const char* tag, size_t size);
void memory_arena_free(MemoryArena* arena);
void* memory_arena_alloc(MemoryArena* arena, size_t element_count, size_t element_size);
void memory_arena_reset(MemoryArena* arena);

} //namespace
} //namespace
