#pragma once

#include "cvDevice.h"

#include <memory>
#include <unordered_map>

namespace CV {

class cvDescriptorSetLayout {
public:
  class Builder {
  public:
    Builder(cvDevice &device) : device(device) {}

    Builder &addBinding(uint32_t binding, VkDescriptorType descriptorType,
                        VkShaderStageFlags stageFlags, uint32_t count = 1);
    std::unique_ptr<cvDescriptorSetLayout> build() const;

  private:
    cvDevice &device;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
  };

  cvDescriptorSetLayout(
      cvDevice &device,
      std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
  ~cvDescriptorSetLayout();

  cvDescriptorSetLayout(const cvDescriptorSetLayout &) = delete;
  cvDescriptorSetLayout &operator=(const cvDescriptorSetLayout &) = delete;

  VkDescriptorSetLayout getDescriptorSetLayout() const {
    return descriptorSetLayout;
  }

private:
  cvDevice &device;
  VkDescriptorSetLayout descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

  friend class cvDescriptorWriter;
};

class cvDescriptorPool {
public:
  class Builder {
  public:
    Builder(cvDevice &device) : device(device) {}

    Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
    Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
    Builder &setMaxSets(uint32_t count);
    std::unique_ptr<cvDescriptorPool> build() const;

  private:
    cvDevice &device;
    std::vector<VkDescriptorPoolSize> poolSizes{};
    uint32_t maxSets = 1000;
    VkDescriptorPoolCreateFlags poolFlags = 0;
  };

  cvDescriptorPool(cvDevice &device, uint32_t maxSets,
                   VkDescriptorPoolCreateFlags poolFlags,
                   const std::vector<VkDescriptorPoolSize> &poolSizes);
  ~cvDescriptorPool();

  cvDescriptorPool(const cvDescriptorPool &) = delete;
  cvDescriptorPool &operator=(const cvDescriptorPool &) = delete;

  bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout,
                          VkDescriptorSet &descriptor) const;

  void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

  void resetPool();

private:
  cvDevice &device;
  VkDescriptorPool descriptorPool;

  friend class cvDescriptorWriter;
};

class cvDescriptorWriter {
public:
  cvDescriptorWriter(cvDescriptorSetLayout &setLayout, cvDescriptorPool &pool);

  cvDescriptorWriter &writeBuffer(uint32_t binding,
                                  VkDescriptorBufferInfo *bufferInfo);
  cvDescriptorWriter &writeImage(uint32_t binding,
                                 VkDescriptorImageInfo *imageInfo);

  bool build(VkDescriptorSet &set);
  void overwrite(VkDescriptorSet &set);

private:
  cvDescriptorSetLayout &setLayout;
  cvDescriptorPool &pool;
  std::vector<VkWriteDescriptorSet> writes;
};

} // namespace CV
