// fileio.h
#pragma once

#include "system.h"
#include "memory.h"

namespace Asteroids {
namespace System {

struct ByteBuffer {
  uint8_t* bytes;
  size_t size;
};

inline bool is_valid(const ByteBuffer* buffer) {
  return buffer->bytes && buffer->size > 0;
}

class FileIO final {
  DISABLE_COPY_AND_MOVE(FileIO);
public:
  FileIO() = default;
  ~FileIO() = default;

  bool init(MemoryArena* arena);

  ByteBuffer read_bytes(const char* path, size_t alloc_size_bytes);
  size_t write_bytes(const char* path, const ByteBuffer* buffer);

private:
  MemoryArena* file_io_arena;
};

const char* file_path_from_exe_base_path(const char* file_name);

} //namespace	
} //namespace
