#pragma once

#include "cvCamera.h"
#include "cvGameObject.h"

#include <vulkan/vulkan.h>

namespace CV {

struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  cvCamera &camera;
  VkDescriptorSet globalDescriptorSet;
  cvGameObject::Map &gameObjects;
};

} // namespace CV
