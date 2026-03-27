#pragma once

#include "cvDevice.h"
#include "cvFrameInfo.h"
#include "cvGameObject.h"
#include "cvPipeline.h"

#include <memory>

namespace CV {

class SimpleRenderSystem {
public:
  SimpleRenderSystem(cvDevice &device, VkRenderPass renderPass,
                     VkDescriptorSetLayout globalSetLayout);
  ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem &) = delete;
  SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

  void renderGameObjects(FrameInfo &frameInfo,
                         std::vector<cvGameObject> &gameObjects);

private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  cvDevice &device;

  std::unique_ptr<cvPipeline> pipeline;
  VkPipelineLayout pipelineLayout;
};

} // namespace CV
