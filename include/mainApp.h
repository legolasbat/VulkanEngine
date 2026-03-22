#pragma once

#include "cvDevice.h"
#include "cvGameObject.h"
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
  mainApp &operator=(const mainApp &) = delete;

  void run();

private:
  void loadGameObjects();
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void freeCommandBuffers();
  void drawFrame();
  void recreateSwapChain();
  void recordCommandBuffer(int imageIndex);
  void renderGameObjects(VkCommandBuffer commandBuffer);

  cvWindow window{WIDTH, HEIGHT, "Main App"};
  cvDevice device{window};
  std::unique_ptr<cvSwapChain> swapChain;
  std::unique_ptr<cvPipeline> pipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
  std::vector<cvGameObject> gameObjects;
};

} // namespace CV
