#pragma once

#include "cvModel.h"

#include <memory>

namespace CV {

struct Transform2dComponent {
  glm::vec2 translation{};
  glm::vec2 scale{1.0f, 1.0f};
  float rotation;

  glm::mat2 mat2() {
    const float s = glm::sin(rotation);
    const float c = glm::cos(rotation);
    glm::mat2 rotMatrix{{c, s}, {-s, c}};

    glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};
    return rotMatrix * scaleMat;
  }
};

class cvGameObject {
public:
  using id_t = unsigned int;

  static cvGameObject createGameObject() {
    static id_t currentId = 0;
    return cvGameObject{currentId++};
  }

  cvGameObject(const cvGameObject &) = delete;
  cvGameObject &operator=(const cvGameObject) = delete;
  cvGameObject(cvGameObject &&) = default;
  cvGameObject &operator=(cvGameObject &&) = default;

  const id_t getId() { return id; }

  std::shared_ptr<cvModel> model{};
  glm::vec3 color{};
  Transform2dComponent transform2d;

private:
  cvGameObject(id_t objId) : id{objId} {}

  id_t id;
};

} // namespace CV
