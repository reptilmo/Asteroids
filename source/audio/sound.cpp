// sound.cpp
#include "sound.h"
#include "system/system.h"

namespace Asteroids {
namespace Audio {

bool SoundPlayer::init(int channels) {
  if (Mix_Init(MIX_INIT_MP3) <= 0) {
    System::log_error("Failed to init MP3 playback: %s", SDL_GetError());
  }

  if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512) < 0) {
    System::log_error("Failed to open audio: %s", SDL_GetError());
    return false;
  }

  channels_ = Mix_AllocateChannels(channels);
  if (channels_ > 0) {
    System::log_info("Allocated [%d] audio channels", channels_);
  } else {
    System::log_error("Failed to allocate audio channels: %s", SDL_GetError());
    return false;
  }

  return true;
}

void SoundPlayer::finalize() {
  for (size_t i = 0; i < sample_count_; i++) {
    Mix_FreeChunk(samples_[i]);
  }

  Mix_CloseAudio();
}

int SoundPlayer::load_wav(const char* file_name) {
  ASSERT(sample_count_ < MAX_SOUND_SAMPLES);
  if (sample_count_ >= MAX_SOUND_SAMPLES) {
    System::log_error("Failed to load wav, max is [%d]", MAX_SOUND_SAMPLES);
    return -1;
  }

  samples_[sample_count_] = Mix_LoadWAV(file_name);
  if (samples_[sample_count_] == nullptr) {
    System::log_error("Failed to load wav: %s", SDL_GetError());
    return -1;
  }

  const int index = (int)sample_count_;
  sample_count_++; 

  return index;
}

void SoundPlayer::play_sound(int index) {
  ASSERT(index >= 0 && index < sample_count_);
  Mix_PlayChannel(-1, samples_[index], 0);
}

bool SoundPlayer::load_mp3(const char* file_name) {
  if (music) {
    Mix_FreeMusic(music);
  }

  music = Mix_LoadMUS(file_name);
  if (!music) {
    System::log_error("Failed to load music file: %s: %s", file_name, SDL_GetError());
    return false;
  }

  Mix_VolumeMusic(10);
  return true;
}

void SoundPlayer::play_music() {
  ASSERT(music);
  Mix_PlayMusic(music, -1);
}

} //namespace
} //namespace
