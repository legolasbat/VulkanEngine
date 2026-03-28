#pragma once

#include "cvModel.h"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace CV {

struct TransformComponent {
  glm::vec3 translation{};
  glm::vec3 scale{1.0f, 1.0f, 1.0f};
  glm::vec3 rotation{};

  // Matrix corresponds to translate * Ry * Rx * Rz * scale transformation
  // Rotation convention uses Tait-Bryan angles with axis order Y(1), X(2), Z(3)
  glm::mat4 mat4();

  glm::mat3 normalMatrix();
};

class cvGameObject {
public:
  using id_t = unsigned int;
  using Map = std::unordered_map<id_t, cvGameObject>;

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
  TransformComponent transform{};

private:
  cvGameObject(id_t objId) : id{objId} {}

  id_t id;
};

} // namespace CV
