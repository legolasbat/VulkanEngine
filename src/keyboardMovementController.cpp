#include "keyboardMovementController.h"

namespace CV {

KeyboardMovementController::KeyboardMovementController(cvInputSystem &input)
    : input(input) {}

void KeyboardMovementController::moveInPlaneXZ(float dt,
                                               cvGameObject &gameObject) {
  glm::vec3 rotate(0);

  if (input.isKeyPressed(keys.lookRight))
    rotate.y += 1.0f;
  if (input.isKeyPressed(keys.lookLeft))
    rotate.y -= 1.0f;
  if (input.isKeyPressed(keys.lookUp))
    rotate.x += 1.0f;
  if (input.isKeyPressed(keys.lookDown))
    rotate.x -= 1.0f;

  if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
    gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);

  gameObject.transform.rotation.x =
      glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
  gameObject.transform.rotation.y =
      glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

  float yaw = gameObject.transform.rotation.y;
  const glm::vec3 forwardDir(sin(yaw), 0.0f, cos(yaw));
  const glm::vec3 rightDir(forwardDir.z, 0.0f, -forwardDir.x);
  const glm::vec3 upDir(0.0f, -1.0f, 0.0f);

  glm::vec3 moveDir(0.0f);
  if (input.isKeyPressed(keys.moveForward))
    moveDir += forwardDir;
  if (input.isKeyPressed(keys.moveBackward))
    moveDir -= forwardDir;
  if (input.isKeyPressed(keys.moveRight))
    moveDir += rightDir;
  if (input.isKeyPressed(keys.moveLeft))
    moveDir -= rightDir;
  if (input.isKeyPressed(keys.moveUp))
    moveDir += upDir;
  if (input.isKeyPressed(keys.moveDown))
    moveDir -= upDir;

  if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
    gameObject.transform.translation +=
        moveSpeed * dt * glm::normalize(moveDir);
}

} // namespace CV
