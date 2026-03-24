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

mainApp::mainApp() { loadGameObjects(); }

mainApp::~mainApp() {}

void mainApp::run() {
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
      renderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
      renderer.endSwapChainRenderPass(commandBuffer);
      renderer.endFrame();
    }
  }

  vkDeviceWaitIdle(device.device());
}

std::unique_ptr<cvModel> createCubeModel(cvDevice &device, glm::vec3 offset) {
  std::vector<cvModel::Vertex> vertices{
      // Left face (white)
      {{-0.5f, -0.5f, -0.5f}, {0.9f, 0.9f, 0.9f}},
      {{-0.5f, 0.5f, 0.5f}, {0.9f, 0.9f, 0.9f}},
      {{-0.5f, -0.5f, 0.5f}, {0.9f, 0.9f, 0.9f}},
      {{-0.5f, -0.5f, -0.5f}, {0.9f, 0.9f, 0.9f}},
      {{-0.5f, 0.5f, -0.5f}, {0.9f, 0.9f, 0.9f}},
      {{-0.5f, 0.5f, 0.5f}, {0.9f, 0.9f, 0.9f}},
      // Right face (yellow)
      {{0.5f, -0.5f, -0.5f}, {0.8f, 0.8f, 0.1f}},
      {{0.5f, 0.5f, 0.5f}, {0.8f, 0.8f, 0.1f}},
      {{0.5f, -0.5f, 0.5f}, {0.8f, 0.8f, 0.1f}},
      {{0.5f, -0.5f, -0.5f}, {0.8f, 0.8f, 0.1f}},
      {{0.5f, 0.5f, -0.5f}, {0.8f, 0.8f, 0.1f}},
      {{0.5f, 0.5f, 0.5f}, {0.8f, 0.8f, 0.1f}},
      // Top face (orange)
      {{-0.5f, -0.5f, -0.5f}, {0.9f, 0.6f, 0.1f}},
      {{0.5f, -0.5f, 0.5f}, {0.9f, 0.6f, 0.1f}},
      {{-0.5f, -0.5f, 0.5f}, {0.9f, 0.6f, 0.1f}},
      {{-0.5f, -0.5f, -0.5f}, {0.9f, 0.6f, 0.1f}},
      {{0.5f, -0.5f, -0.5f}, {0.9f, 0.6f, 0.1f}},
      {{0.5f, -0.5f, 0.5f}, {0.9f, 0.6f, 0.1f}},
      // Bottom face (red)
      {{-0.5f, 0.5f, -0.5f}, {0.8f, 0.1f, 0.1f}},
      {{0.5f, 0.5f, 0.5f}, {0.8f, 0.1f, 0.1f}},
      {{-0.5f, 0.5f, 0.5f}, {0.8f, 0.1f, 0.1f}},
      {{-0.5f, 0.5f, -0.5f}, {0.8f, 0.1f, 0.1f}},
      {{0.5f, 0.5f, -0.5f}, {0.8f, 0.1f, 0.1f}},
      {{0.5f, 0.5f, 0.5f}, {0.8f, 0.1f, 0.1f}},
      // Front face (blue)
      {{-0.5f, -0.5f, 0.5f}, {0.1f, 0.1f, 0.8f}},
      {{0.5f, 0.5f, 0.5f}, {0.1f, 0.1f, 0.8f}},
      {{-0.5f, 0.5f, 0.5f}, {0.1f, 0.1f, 0.8f}},
      {{-0.5f, -0.5f, 0.5f}, {0.1f, 0.1f, 0.8f}},
      {{0.5f, -0.5f, 0.5f}, {0.1f, 0.1f, 0.8f}},
      {{0.5f, 0.5f, 0.5f}, {0.1f, 0.1f, 0.8f}},
      // Back face (green)
      {{-0.5f, -0.5f, -0.5f}, {0.1f, 0.8f, 0.1f}},
      {{0.5f, 0.5f, -0.5f}, {0.1f, 0.8f, 0.1f}},
      {{-0.5f, 0.5f, -0.5f}, {0.1f, 0.8f, 0.1f}},
      {{-0.5f, -0.5f, -0.5f}, {0.1f, 0.8f, 0.1f}},
      {{0.5f, -0.5f, -0.5f}, {0.1f, 0.8f, 0.1f}},
      {{0.5f, 0.5f, -0.5f}, {0.1f, 0.8f, 0.1f}},
  };

  for (auto &v : vertices) {
    v.position += offset;
  }

  return std::make_unique<cvModel>(device, vertices);
}

void mainApp::loadGameObjects() {
  std::shared_ptr<cvModel> model = createCubeModel(device, {0.0f, 0.0f, 0.0f});

  auto cube = cvGameObject::createGameObject();
  cube.model = model;
  cube.transform.translation = {0.0f, 0.0f, 2.5f};
  cube.transform.scale = {0.5f, 0.5f, 0.5f};
  gameObjects.push_back(std::move(cube));
}

} // namespace CV
