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

void mainApp::loadGameObjects() {
  std::shared_ptr<cvModel> model =
      cvModel::createModelFromFile(device, "resources/models/smooth_vase.obj");

  auto gameObj = cvGameObject::createGameObject();
  gameObj.model = model;
  gameObj.transform.translation = {0.0f, 0.0f, 2.5f};
  gameObj.transform.scale = glm::vec3(3.0f);
  gameObjects.push_back(std::move(gameObj));
}

} // namespace CV
