#include "mainApp.h"

#include <array>
#include <stdexcept>

namespace CV {

mainApp::mainApp() {
  loadModels();
  createPipelineLayout();
  recreateSwapChain();
  createCommandBuffers();
}

mainApp::~mainApp() {
  vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void mainApp::run() {
  while (!window.shouldClose()) {
    window.pollEvents();
    drawFrame();
  }

  vkDeviceWaitIdle(device.device());
}
void mainApp::loadModels() {
  std::vector<cvModel::Vertex> vertices{{{0.0f, -0.5}, {1.0f, 0.0f, 0.0f}},
                                        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

  model = std::make_unique<cvModel>(device, vertices);
}

void mainApp::createPipelineLayout() {
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;
  if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create pipeline layout!");
  }
}

void mainApp::createPipeline() {
  auto pipelineConfig = cvPipeline::defaultPipelineConfigInfo(
      swapChain->width(), swapChain->height());
  pipelineConfig.renderPass = swapChain->getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  pipeline = std::make_unique<cvPipeline>(
      device, "resources/shaders/simple_shader.vert.spv",
      "resources/shaders/simple_shader.frag.spv", pipelineConfig);
}

void mainApp::recreateSwapChain() {
  auto extent = window.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = window.getExtent();
    SDL_Event event;
    SDL_WaitEvent(&event);
  }

  vkDeviceWaitIdle(device.device());
  swapChain = std::make_unique<cvSwapChain>(device, extent);
  createPipeline();
}

void mainApp::createCommandBuffers() {
  commandBuffers.resize(swapChain->imageCount());

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = device.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(device.device(), &allocInfo,
                               commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate command buffers!");
  }
}

void mainApp::recordCommandBuffer(int imageIndex) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = swapChain->getRenderPass();
  renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  pipeline->bind(commandBuffers[imageIndex]);
  model->bind(commandBuffers[imageIndex]);
  model->draw(commandBuffers[imageIndex]);

  vkCmdEndRenderPass(commandBuffers[imageIndex]);
  if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
    throw std::runtime_error("Failed to record command buffer!");
  }
}

void mainApp::drawFrame() {
  uint32_t imageIndex;
  auto result = swapChain->acquireNextImage(&imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("Failed to acquire swap chain image!");
  }

  recordCommandBuffer(imageIndex);
  result =
      swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      window.wasWindowResized()) {
    window.resetWindowResizedFlag();
    recreateSwapChain();
    return;
  }
  if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to present swap chain image!");
  }
}

} // namespace CV
