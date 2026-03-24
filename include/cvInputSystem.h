#pragma once

#include <SDL3/SDL.h>

namespace CV {

struct KeyboardState {
  bool keys[SDL_SCANCODE_COUNT];
};

class cvInputSystem {
public:
  void pollEvents();

  bool quitRequested() { return quitEvent; }

  bool isKeyPressed(SDL_Scancode scancode) {
    return keyboardState.keys[scancode];
  }

private:
  bool quitEvent = false;

  KeyboardState keyboardState;
};

} // namespace CV
