#include "Plane.hpp"

#include <algorithm>

Plane::Plane(glm::vec3 A, glm::vec3 B, glm::vec3 C) {

    /* Calculate A, B, C, D, such that Ax + By +Cz + D =0 */
    glm::vec3 AB = B - A;
    glm::vec3 AC = C - A;
    normal_ = glm::normalize(glm::cross(AB, AC));

    A_ = normal_.x;
    B_ = normal_.y;
    C_ = normal_.z;

    D_ = -(A_*A.x + B_ * A.y + C_ * A.z);
    
    /* Calculate quadric matrix for this plane */
    Kp_[0][0] = A_ * A_;
    Kp_[0][1] = A_ * B_;
    Kp_[0][2] = A_ * C_;
    Kp_[0][3] = A_ * D_;

    Kp_[1][0] = A_ * B_;
    Kp_[1][1] = B_ * B_;
    Kp_[1][2] = B_ * C_;
    Kp_[1][3] = B_ * D_;

    Kp_[2][0] = A_ * C_;
    Kp_[2][1] = B_ * C_;
    Kp_[2][2] = C_ * C_;
    Kp_[2][3] = C_ * D_;

    Kp_[3][0] = A_ * D_;
    Kp_[3][1] = B_ * D_;
    Kp_[3][2] = C_ * D_;
    Kp_[3][3] = D_ * D_;
}

