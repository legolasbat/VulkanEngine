#pragma once

#include "cvDevice.h"
#include "cvGameObject.h"
#include "cvInputSystem.h"
#include "cvRenderer.h"
#include "cvWindow.h"

namespace CV {

class mainApp {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  mainApp();
  ~mainApp();

  mainApp(const mainApp &) = delete;
  mainApp &operator=(const mainApp &) = delete;

  void run();

private:
  void loadGameObjects();

  cvWindow window{WIDTH, HEIGHT, "Main App"};
  cvDevice device{window};
  cvInputSystem input{};
  cvRenderer renderer{window, device, input};

  std::vector<cvGameObject> gameObjects;
};

} // namespace CV
