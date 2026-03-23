#include "mainApp.h"

#include "simpleRenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace CV {

mainApp::mainApp() { loadGameObjects(); }

mainApp::~mainApp() {}

void mainApp::run() {
  SimpleRenderSystem simpleRenderSystem(device,
                                        renderer.getSwapChainRenderPass());

  while (!window.shouldClose()) {
    window.pollEvents();

    if (auto commandBuffer = renderer.beginFrame()) {
      renderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
      renderer.endSwapChainRenderPass(commandBuffer);
      renderer.endFrame();
    }
  }

  vkDeviceWaitIdle(device.device());
}
void mainApp::loadGameObjects() {
  std::vector<cvModel::Vertex> vertices{{{0.0f, -0.5}, {1.0f, 0.0f, 0.0f}},
                                        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

  auto model = std::make_shared<cvModel>(device, vertices);

  auto triangle = cvGameObject::createGameObject();
  triangle.model = model;
  triangle.color = {0.1f, 0.8f, 0.1f};
  triangle.transform2d.translation.x = 0.2f;
  triangle.transform2d.scale = {2.0f, 0.5f};
  triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

  gameObjects.push_back(std::move(triangle));
}

} // namespace CV
