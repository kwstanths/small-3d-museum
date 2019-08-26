#ifndef __TimeCriticalRendering_hpp__
#define __TimeCriticalRendering_hpp__

#include <vector>

#include "GraphicsObject.hpp"
#include "TriangleMesh.h"

class TimeCriticalRendering {
public:
    TimeCriticalRendering();


    void AddObject(GraphicsObject * object);

    void ClearObjects();

    void Step(glm::vec3 camera_position, double frame_time);

    void ComputeTPS(size_t triangles_rendered, double frame_time);

    ~TimeCriticalRendering();

private:
    std::vector<GraphicsObject *> objects_;
    
    std::vector<size_t> TPSs_;
    size_t TPS_index_ = 0;

    size_t TPS_;

    size_t last_frame_triangles_rendered_ = 0;

    float CalculateBenefit(GraphicsObject * object, GraphicsObject::LodLevel lod, glm::vec3 camera_position);
};

#endif