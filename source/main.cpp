// main.cpp
#include "game/global.h"
#include "game/loop.h"

using namespace Asteroids;

Game::Global global;

int main(int argc, char* argv[]) {

  System::FileIO file_io;
  file_io.init(global.file_io_arena);
  auto buffer = file_io.read_bytes(System::file_path_from_exe_base_path("Asteroids.conf"), System::KB(1));

  System::ConfigMap config;
  config.init(&buffer);
  config.parse_command_line(argc, argv);

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
    System::log_error("Error from SDL init!");
    return 1;
  }

  atexit(SDL_Quit);

  if (!global.init(&config)) {
    global.finalize();
    return 1;
  }

  Game::Loop loop;
  if (loop.init(&global)) {
    loop.run();
  }

  loop.finalize();
  global.finalize();

  SDL_Quit();
  return 0;
}
