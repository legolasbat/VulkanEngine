#include "cvInputSystem.h"

namespace CV {

void cvInputSystem::pollEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      quitEvent = true;
    } else if (event.type == SDL_EVENT_KEY_DOWN) {
      keyboardState.keys[event.key.scancode] = true;
    } else if (event.type == SDL_EVENT_KEY_UP) {
      keyboardState.keys[event.key.scancode] = false;
    }
  }
}

} // namespace CV
