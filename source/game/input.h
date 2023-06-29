 // input.h
#pragma once

#include "system/system.h"
#include "system/config.h"

namespace Asteroids {
namespace Game {

class InputHandler final {
  DISABLE_COPY_AND_MOVE(InputHandler);
public:
  InputHandler() = default;
  ~InputHandler() = default;

  void init(const System::ConfigMap* config);
  void update();

  int quit_pressed() const { return keys_pressed_new[key_quit]; }
  int impulse_pressed() const { return keys_pressed_new[key_impulse]; }
  int shoot_pressed() const { return keys_pressed_new[key_shoot]; }
  int rotate_left_pressed() const { return keys_pressed_new[key_rotate_left]; }
  int rotate_right_pressed() const { return keys_pressed_new[key_rotate_right]; }
  int zoom_in_pressed() const { return keys_pressed_new[key_zoom_in]; }
  int zoom_out_pressed() const { return keys_pressed_new[key_zoom_out]; }

  int quit_was_pressed() const { return keys_pressed_old[key_quit]; }
  int impulse_was_pressed() const { return keys_pressed_old[key_impulse]; }
  int shoot_was_pressed() const { return keys_pressed_old[key_shoot]; }
  int rotate_left_was_pressed() const { return keys_pressed_old[key_rotate_left]; }
  int rotate_right_was_pressed() const { return keys_pressed_old[key_rotate_right]; }
  int zoom_in_was_pressed() const { return keys_pressed_old[key_zoom_in]; }
  int zoom_out_was_pressed() const { return keys_pressed_old[key_zoom_out]; }

private:
  uint8_t keys_pressed_new[SDL_NUM_SCANCODES] = {};
  uint8_t keys_pressed_old[SDL_NUM_SCANCODES] = {};

  int key_quit = 41;
  int key_impulse = 98;
  int key_shoot = 44;
  int key_rotate_left = 80;
  int key_rotate_right = 79;
  int key_zoom_in = 86;
  int key_zoom_out = 87;
};

} //namespace
} //namespace
