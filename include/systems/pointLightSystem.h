#pragma once

#include "cvDevice.h"
#include "cvFrameInfo.h"
#include "cvPipeline.h"

#include <memory>

namespace CV {

class PointLightSystem {
public:
  PointLightSystem(cvDevice &device, VkRenderPass renderPass,
                   VkDescriptorSetLayout globalSetLayout);
  ~PointLightSystem();

  PointLightSystem(const PointLightSystem &) = delete;
  PointLightSystem &operator=(const PointLightSystem &) = delete;

  void update(FrameInfo &frameInfo, GlobalUbo &ubo);
  void render(FrameInfo &frameInfo);

private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  cvDevice &device;

  std::unique_ptr<cvPipeline> pipeline;
  VkPipelineLayout pipelineLayout;
};

} // namespace CV
