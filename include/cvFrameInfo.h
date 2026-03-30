#pragma once

#include "cvCamera.h"
#include "cvGameObject.h"

#include <vulkan/vulkan.h>

namespace CV {

#define MAX_LIGHTS 10

struct PointLight {
  glm::vec4 position{};
  glm::vec4 color{};
};

struct GlobalUbo {
  glm::mat4 projection{1.0f};
  glm::mat4 view{1.0f};
  glm::mat4 inverseView{1.0f};
  glm::vec4 ambientLightColor{1.0f, 1.0f, 1.0f, 0.02f}; // W is intensity
  PointLight pointLights[MAX_LIGHTS];
  int numLights;
};

struct FrameInfo {
  int frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  cvCamera &camera;
  VkDescriptorSet globalDescriptorSet;
  cvGameObject::Map &gameObjects;
};

} // namespace CV
