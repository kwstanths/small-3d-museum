#ifndef __Plane_hpp__
#define __Plane_hpp__

#include <glm/glm.hpp>

class Plane {
public:
    Plane() {};
    Plane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

public :
    double A_, B_, C_, D_;
    glm::vec3 normal_;

    glm::dmat4 Kp_;
};


#endif