#include "cvWindow.h"

namespace CV {
cvWindow::cvWindow(int w, int h, std::string name)
    : width(w), height(h), windowName(name) {
  initWindow();

  isOpen = true;
}

void cvWindow::initWindow() {
  // Init systems
  if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s\n", SDL_GetError());
    return;
  }

  // Create window
  float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  SDL_WindowFlags window_flags = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE;
  window = SDL_CreateWindow(windowName.c_str(), width * main_scale,
                            height * main_scale, window_flags);
  if (window == nullptr) {
    SDL_Log("Couldn't create window: %s\n", SDL_GetError());
    SDL_Quit();
    return;
  }
}

void cvWindow::pollEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_KEY_DOWN)
      isOpen = false;
  }
}

void cvWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
  if (!SDL_Vulkan_CreateSurface(window, instance, nullptr, surface)) {
    SDL_Log("Couldn't create surface: %s\n", SDL_GetError());
    SDL_Quit();
  }
}

cvWindow::~cvWindow() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}
} // namespace CV
