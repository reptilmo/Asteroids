// memory_win32.cpp
#include "memory.h"

//FIXEME: Move Windows header(s) to system.h! 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace Asteroids {
namespace System {

MemoryArena* memory_arena_create(const char* tag, size_t size) {
	ASSERT(tag);
	ASSERT(size);

	size_t total_size = size + sizeof(MemoryArena);
	LPVOID memory = VirtualAlloc(0, total_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (memory) {
		MemoryArena* arena = (MemoryArena*)memory;
		arena->allocated_size = size;
		arena->bytes = (uint8_t*)arena + sizeof(MemoryArena);
		memcpy(arena->tag, tag, System::min<size_t>(strlen(tag), 8));
		return arena;
	}

	return nullptr;
}

void memory_arena_free(MemoryArena* arena) {
	ASSERT(arena);

	if (!VirtualFree(arena, 0, MEM_RELEASE)) {
		log_error("Failed to free memory arena");
	}
}

void* memory_arena_alloc(MemoryArena* arena, size_t element_count, size_t element_size) {
	ASSERT(element_count > 0 && element_size > 0);

	const size_t alloc_size = element_count * element_size;
  ASSERT((arena->used_size + alloc_size) < arena->allocated_size);
	if ((arena->used_size + alloc_size) > arena->allocated_size) {
		log_error("Request allocation is too larged for the arena");
		return nullptr;
	}

	const size_t used_size = arena->used_size;
	arena->used_size += alloc_size;

	return arena->bytes + used_size;	
}

void memory_arena_reset(MemoryArena* arena) {
	ZeroMemory(arena->bytes, arena->used_size);
	arena->used_size = 0;
}

} //namespace
} //namespace
