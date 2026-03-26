#include "mainApp.h"

#include "cvCamera.h"
#include "keyboardMovementController.h"
#include "simpleRenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <chrono>

namespace CV {

struct GlobalUbo {
  glm::mat4 projectionView{1.0f};
  glm::vec3 lightDirection = glm::normalize(glm::vec3{1.0f, -3.0f, -1.0f});
};

mainApp::mainApp() { loadGameObjects(); }

mainApp::~mainApp() {}

void mainApp::run() {
  std::vector<std::unique_ptr<cvBuffer>> uboBuffers(
      cvSwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < uboBuffers.size(); i++) {
    uboBuffers[i] = std::make_unique<cvBuffer>(
        device, sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    uboBuffers[i]->map();
  }

  SimpleRenderSystem simpleRenderSystem(device,
                                        renderer.getSwapChainRenderPass());
  cvCamera camera{};
  camera.setViewTarget(glm::vec3(-1.0f, -2.0f, 2.0f),
                       glm::vec3(0.0f, 0.0f, 2.5f));

  auto viewerObject = cvGameObject::createGameObject();
  KeyboardMovementController cameraController{input};

  auto currentTime = std::chrono::high_resolution_clock::now();

  while (!window.shouldClose()) {
    input.pollEvents();

    window.setQuit(input.quitRequested());

    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime =
        std::chrono::duration<float, std::chrono::seconds::period>(newTime -
                                                                   currentTime)
            .count();
    currentTime = newTime;

    cameraController.moveInPlaneXZ(frameTime, viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation,
                      viewerObject.transform.rotation);

    float aspect = renderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);

    if (auto commandBuffer = renderer.beginFrame()) {
      int frameIndex = renderer.getFrameIndex();
      FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera};

      // Update
      GlobalUbo ubo{};
      ubo.projectionView = camera.getProjection() * camera.getView();
      uboBuffers[frameIndex]->writeToBuffer(&ubo);
      uboBuffers[frameIndex]->flush();

      // Render
      renderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
      renderer.endSwapChainRenderPass(commandBuffer);
      renderer.endFrame();
    }
  }

  vkDeviceWaitIdle(device.device());
}

void mainApp::loadGameObjects() {
  std::shared_ptr<cvModel> model =
      cvModel::createModelFromFile(device, "resources/models/flat_vase.obj");

  auto flatVase = cvGameObject::createGameObject();
  flatVase.model = model;
  flatVase.transform.translation = {-0.5f, 0.5f, 2.5f};
  flatVase.transform.scale = glm::vec3(3.0f, 1.5f, 3.0f);
  gameObjects.push_back(std::move(flatVase));

  model =
      cvModel::createModelFromFile(device, "resources/models/smooth_vase.obj");
  auto smoothVase = cvGameObject::createGameObject();
  smoothVase.model = model;
  smoothVase.transform.translation = {0.5f, 0.5f, 2.5f};
  smoothVase.transform.scale = glm::vec3(3.0f, 1.5f, 3.0f);
  gameObjects.push_back(std::move(smoothVase));
}

} // namespace CV
