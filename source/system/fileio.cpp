// fileio.cpp
#include "fileio.h"
#include "memory.h"

namespace Asteroids {
namespace System {

constexpr size_t FILE_PATH_LENGTH = 1024;
constexpr size_t FILE_CHUNK_SIZE = 4096;

const char* file_path_from_exe_base_path(const char* file_name) {
  thread_local static char file_path[FILE_PATH_LENGTH] = {};
  const char* base_path = SDL_GetBasePath();
  const size_t base_path_len = SDL_utf8strlcpy(file_path, base_path, FILE_PATH_LENGTH);
  
  if (base_path_len + strlen(file_name) >= FILE_PATH_LENGTH) {
    log_error("File path [%s] is too long!");
    return nullptr;
  }

  const size_t len = SDL_strlcat(file_path, file_name, base_path_len + strlen(file_name) + 1);
  file_path[len] = '\0';

  return file_path;
}

bool FileIO::init(MemoryArena* arena) {
  ASSERT(arena && arena->allocated_size > 0);
  if (!arena || !arena->allocated_size) {
    return false;
  }

  file_io_arena = arena;
  return true;
}

ByteBuffer FileIO::read_bytes(const char* path, size_t alloc_size_bytes) {
  ASSERT(file_io_arena);
  ASSERT(path);

  ByteBuffer buffer = {};

  FILE* file = fopen(path, "rb");
  if (!file || ferror(file)) {
    log_error("Failed to open file [%s]", path);
    return buffer;
  }

  buffer.bytes = (uint8_t*)memory_arena_alloc(file_io_arena, alloc_size_bytes, sizeof(uint8_t));
  if (buffer.bytes) {
    buffer.size = alloc_size_bytes;
  }

  size_t read_size = 0;

  do {
    const size_t size = fread(buffer.bytes + read_size, sizeof(uint8_t), FILE_CHUNK_SIZE, file);
    if (size <= 0) {
      break;
    }

    read_size += size;

  } while ((buffer.bytes + read_size) < (buffer.bytes + buffer.size));

  fclose(file);

  buffer.size = read_size;
  return buffer;
}

size_t FileIO::write_bytes(const char* path, const ByteBuffer* buffer) {
  return 0;
}

} //namespace	
} //namespace
