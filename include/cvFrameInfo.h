#pragma once

#include "cvCamera.h"

#include <vulkan/vulkan.h>

namespace CV {

struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  cvCamera &camera;
};

} // namespace CV
