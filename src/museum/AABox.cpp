#include "AABox.hpp"

AABox::AABox() {
    min_ = glm::vec3(-1, -1, -1);
    max_ = glm::vec3(1, 1, 1);
}

AABox::AABox(glm::vec3 min, glm::vec3 max) {
    min_ = min;
    max_ = max;
}

float AABox::GetDiagonalDistance() {
    return glm::distance(min_, max_);
}
