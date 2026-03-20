#pragma once

#include "cvDevice.h"
#include "cvPipeline.h"
#include "cvSwapChain.h"
#include "cvWindow.h"
#include <memory>

namespace CV {

class mainApp {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  mainApp();
  ~mainApp();

  mainApp(const mainApp &) = delete;
  mainApp operator=(const mainApp &) = delete;

  void run();

private:
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void drawFrame();

  cvWindow window{WIDTH, HEIGHT, "Main App"};
  cvDevice device{window};
  cvSwapChain swapChain{device, window.getExtent()};
  std::unique_ptr<cvPipeline> pipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
};

} // namespace CV
