// config.cpp
#include "config.h"
#include "system.h"

namespace Asteroids {
namespace System {

constexpr size_t MAX_LINE_LENGTH = 256;

static inline void remove_white_space(char* str) {
  char* str2 = str;
  do {
    while (isspace(*str2)) {
      ++str2;
    }
  } while (*str++ = *str2++);
}

void ConfigMap::init(const ByteBuffer* buffer) {
  memset(keys_, 0, sizeof(keys_));
  memset(values_, 0, sizeof(values_));

  if (!is_valid(buffer)) {
    return;
  }

  const char* buffer_end = (char*)buffer->bytes + buffer->size;
  const char* line_start = (char*)buffer->bytes;
  size_t line_len = 0;
  char line[MAX_LINE_LENGTH];
  
  while (line_start + line_len < buffer_end) {
    line_start += line_len;
    line_len = 0;

    while (*(line_start + line_len) != '\n' && line_start + line_len < buffer_end) {
      line_len++;
    }
    line_len++;

    if (*line_start == '#') {
      continue;
    }

    memset(line, 0, MAX_LINE_LENGTH);
    memcpy(line, line_start, System::min(line_len - 1, MAX_LINE_LENGTH - 1));

    if (strlen(line) == 0) {
      continue;
    }

    remove_white_space(line);
    const char* key = strtok(line, "=");
    if (strlen(key) >= CONFIG_MAP_KEY_LEN) {
      log_info("Confing key [%s] is too long!", key);
      continue;
    }

    const char* value = strtok(NULL, "=");
    if (strlen(value) >= CONFIG_MAP_VAL_LEN) {
      log_info("Config value [%s] is too long!", value);
      continue;
    }

    strncpy(keys_[key_count_], key, strlen(key));
    strncpy(values_[key_count_], value, strlen(value));
    key_count_++;
  }
}

const char* ConfigMap::value_str(const char* key, const char* default_value) const {
  const size_t len = min(strlen(key), CONFIG_MAP_KEY_LEN);
	for (size_t i = 0; i < key_count_; i++) {
    if (strncmp(keys_[i], key, len) == 0) {
      return values_[i];
    }
  }

  return default_value;
}

int ConfigMap::value_int(const char* key, int default_value) const {
  const char* value = value_str(key, NULL);
  if (value != NULL) {
    return atoi(value);
  }

  return default_value;
}

float ConfigMap::value_float(const char* key, float default_value) const {
  const char* value = value_str(key, NULL);
  if (value != NULL) {
    return strtof(value, NULL);
  }

  return default_value;
}

void ConfigMap::set_value(const char* key, const char* value) {
  if (key == NULL || value == NULL) {
    return;
  }

  if (strlen(key) >= CONFIG_MAP_KEY_LEN) {
    log_info("Confing key [%s] is too long!", key);
    return;
  }

  if (strlen(value) >= CONFIG_MAP_VAL_LEN) {
    log_info("Config value [%s] is too long!", value);
    return;
  }

  for (size_t i = 0; i < key_count_; i++) {
    if (strncmp(keys_[i], key, strlen(key)) == 0) {
      memset(values_[i], 0, CONFIG_MAP_VAL_LEN);
      strncpy(values_[i], value, strlen(value));
      return;
    }
  }

  if (key_count_ < CONFIG_MAP_MAX_KEYS) {
    strncpy(keys_[key_count_], key, strlen(key));
    strncpy(values_[key_count_], value, strlen(value));
    key_count_++;
  }
}

void ConfigMap::parse_command_line(int argc, char* argv[]) {

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--fullscreen") == 0) {
      set_value("fullscreen", "true");
      continue;
    }

    const char* param_key = strtok(argv[i], "=");

    if (strncmp(param_key, "--w", 3) == 0) {
      const char* param_value = strtok(NULL, "=");
      set_value("window_width", param_value);
    } else if (strncmp(param_key, "--h", 3) == 0) {
      const char* param_value = strtok(NULL, "=");
      set_value("window_height", param_value);
    } else if (strncmp(argv[i], "--mesh", 6) == 0) {
      const char* param_value = strtok(NULL, "=");
      set_value("load_mesh", param_value);
    } else if (strncmp(argv[i], "--texture", 9) == 0) {
      const char* param_value = strtok(NULL, "=");
      set_value("load_texture", param_value);
    }
  }
}

} //namespace
} //namespace
