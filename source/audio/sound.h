// sound.h
#pragma once 

#include "system/system.h"

#include <SDL_mixer.h>

namespace Asteroids {
namespace Audio {

constexpr size_t MAX_SOUND_SAMPLES = 16;

class SoundPlayer final {
  DISABLE_COPY_AND_MOVE(SoundPlayer);
public:
  SoundPlayer() = default;
  ~SoundPlayer() = default;

  bool init(int channels);
  void finalize();

  int load_wav(const char* file_name);
  void play_sound(int sample_index);

  bool load_mp3(const char* file_name);
  void play_music();

private:
  int channels_ = 0;
  Mix_Chunk* samples_[MAX_SOUND_SAMPLES] = {};
  size_t sample_count_ = 0;
  Mix_Music* music = nullptr;
};

} //namespace
} //namespace
