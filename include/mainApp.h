#pragma once

#include "cvDevice.h"
#include "cvModel.h"
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
  void loadModels();
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void drawFrame();

  void sierpinski(std::vector<cvModel::Vertex> &vertices, int depth,
                  glm::vec2 left, glm::vec2 right, glm::vec2 top);

  cvWindow window{WIDTH, HEIGHT, "Main App"};
  cvDevice device{window};
  cvSwapChain swapChain{device, window.getExtent()};
  std::unique_ptr<cvPipeline> pipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
  std::unique_ptr<cvModel> model;
};

} // namespace CV
