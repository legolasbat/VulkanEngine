#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <vulkan/vulkan.h>

#include <string>

namespace CV {

class cvWindow {
public:
  cvWindow(int w, int h, std::string name);

  ~cvWindow();

  cvWindow(const cvWindow &) = delete;
  cvWindow &operator=(const cvWindow &) = delete;

  bool shouldClose() { return !isOpen; };

  void pollEvents();

  VkExtent2D getExtent() {
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
  }
  bool wasWindowResized() { return framebufferResized; }
  void resetWindowResizedFlag() { framebufferResized = false; }

  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
  void initWindow();

  int width;
  int height;
  bool framebufferResized = false;

  bool isOpen;

  std::string windowName;

  SDL_Window *window;
};

} // namespace CV
