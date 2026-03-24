#pragma once

#include "cvDevice.h"
#include "cvSwapChain.h"
#include "cvWindow.h"

#include <cassert>
#include <memory>

namespace CV {

class cvRenderer {
public:
  cvRenderer(cvWindow &window, cvDevice &device);
  ~cvRenderer();

  cvRenderer(const cvRenderer &) = delete;
  cvRenderer &operator=(const cvRenderer &) = delete;

  VkRenderPass getSwapChainRenderPass() const {
    return swapChain->getRenderPass();
  }
  float getAspectRatio() const { return swapChain->extentAspectRatio(); }

  bool isFrameInProgress() const { return isFrameStarted; }

  VkCommandBuffer getCurrentCommandBuffer() const {
    assert(isFrameStarted &&
           "Cannot get command buffer when frame not in progress");
    return commandBuffers[currentFrameIndex];
  }

  int getFrameIndex() const {
    assert(isFrameStarted &&
           "Cannot get frame index when frame not in progress");
    return currentImageIndex;
  }

  VkCommandBuffer beginFrame();
  void endFrame();
  void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
  void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
  void createCommandBuffers();
  void freeCommandBuffers();
  void recreateSwapChain();

  cvWindow &window;
  cvDevice &device;
  std::unique_ptr<cvSwapChain> swapChain;
  std::vector<VkCommandBuffer> commandBuffers;

  uint32_t currentImageIndex;
  int currentFrameIndex;
  bool isFrameStarted;
};

} // namespace CV
