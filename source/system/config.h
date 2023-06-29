// config.h
#pragma once

#include "fileio.h"

namespace Asteroids {
namespace System {

constexpr size_t CONFIG_MAP_MAX_KEYS = 64;
constexpr size_t CONFIG_MAP_KEY_LEN = 31;
constexpr size_t CONFIG_MAP_VAL_LEN = 41;

class ConfigMap final {
  DISABLE_COPY_AND_MOVE(ConfigMap);
public:
  ConfigMap() = default;
  ~ConfigMap() = default;

  void init(const ByteBuffer* buffer);
  void parse_command_line(int argc, char* argv[]);

  const char* value_str(const char* key, const char* default_value) const;
  int value_int(const char* key, int default_value) const;
  float value_float(const char* key, float default_value) const;

  void set_value(const char* key, const char* value);
private:
  char keys_[CONFIG_MAP_MAX_KEYS][CONFIG_MAP_KEY_LEN];
  char values_[CONFIG_MAP_MAX_KEYS][CONFIG_MAP_VAL_LEN];
  size_t key_count_ = 0;
};

} //namespace
} //namespace
