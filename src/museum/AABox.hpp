#ifndef __AABox_hpp__
#define __AABox_hpp__

#include "glm/glm.hpp"

class AABox {
public:

    AABox();
    AABox(glm::vec3 min, glm::vec3 max);

    float GetDiagonalDistance();

    glm::vec3 min_;
    glm::vec3 max_;
private:
};


#endif