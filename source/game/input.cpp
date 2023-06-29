// input.cpp
#include "input.h"


namespace Asteroids {
namespace Game {

void InputHandler::init(const System::ConfigMap* config) {

  key_quit = config->value_int("key_quit", key_quit);
  key_impulse = config->value_int("key_impulse", key_impulse);
  key_shoot = config->value_int("key_shoot", key_shoot);
  key_rotate_left = config->value_int("key_rotate_left", key_rotate_left);
  key_rotate_right = config->value_int("key_rotate_right", key_rotate_right);
  key_zoom_in = config->value_int("key_zoom_in", key_zoom_in);
  key_zoom_out = config->value_int("key_zoom_out", key_zoom_out);

  //SDL_SetRelativeMouseMode(SDL_TRUE);
}

void InputHandler::update() {
  memcpy(keys_pressed_old, keys_pressed_new, sizeof(keys_pressed_new));

  int key_count = 0;
  const uint8_t* key_state = SDL_GetKeyboardState(&key_count);
  ASSERT(key_count == SDL_NUM_SCANCODES);

  memcpy(keys_pressed_new, key_state, sizeof(keys_pressed_new));
}

} //namespace
} //namespace
