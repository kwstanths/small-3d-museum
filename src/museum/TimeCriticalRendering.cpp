#include "TimeCriticalRendering.hpp"

#include <algorithm>
#include <iostream>

#include "ConfigurationFile.hpp"

TimeCriticalRendering::TimeCriticalRendering() {
    /* Get the TPS from configuration file */
    TPS_ = ConfigurationFile::instance().GetTPS();
    
    /* If TPS is calcualte on the fly, do 5-way average */
    TPSs_ = std::vector<size_t>(5, TPS_);
}

void TimeCriticalRendering::AddObject(GraphicsObject * object) {
    objects_.push_back(object);
}

void TimeCriticalRendering::ClearObjects() {
    objects_.clear();
}
    
void TimeCriticalRendering::Step(glm::vec3 camera_position, double frame_time) {

    /**
        This calculation does not work! If we have a scene where only some
        meshes are visible with a small number of triangles, then the TPS
        will end up to be billions of triangles. This creates stuttering 
        and frame drops, that go away as the TPS adapts to the heavier 
        scenes and gets closer to the real number. This cycle happens again 
        and again, and again...
    */
    //if (last_frame_triangles_rendered_ !=0) ComputeTPS(last_frame_triangles_rendered_, frame_time);

    float max_triangles_per_frame = TPS_ / 60.0f;
    
    /* We accumulate the triangles here */
    float current_triangles = 0.0f;

    /* Find the objects that will have their LOD re-calculated */
    size_t n_objects = 0;
    /* Holds the objects allowed to change LOD based on their last time that they changed LOD */
    std::vector<GraphicsObject *> objects_temp(objects_.size());
    for (size_t i = 0; i < objects_.size(); i++) {
        if (objects_[i]->CanChangeLod()) objects_temp[n_objects++] = objects_[i];
        else current_triangles += objects_[i]->GetChosenLodMesh()->GetNumTriangles();
    }
    
    /* 
        Holds the new lod level for the objects that can change lod 
        Set the lower possible LOD level to them, accumulate the cost 
    */
    std::vector<GraphicsObject::LodLevel> new_lod_levels(n_objects, GraphicsObject::LodLevel::THREE);
    for (size_t i = 0; i < n_objects; i++) {
        current_triangles += objects_temp[i]->GetLodMesh(GraphicsObject::LodLevel::THREE)->GetNumTriangles();
    }
    
    /* Accumulates the triangles that will be sent to the GPU */
    last_frame_triangles_rendered_ = current_triangles;
    while (1) {

        float best_benefit_delta = -std::numeric_limits<float>::max();
        int object_to_upgrade = -1;
        for (size_t i = 0; i < n_objects; i++) {
            /* If at maximum level already, skip it */
            if (new_lod_levels[i] == GraphicsObject::LodLevel::ONE) continue;

            /* Calculate old and new benefit for this object */
            GraphicsObject::LodLevel new_level = static_cast<GraphicsObject::LodLevel>(new_lod_levels[i] - 1);
            float old_benefit = CalculateBenefit(objects_temp[i], new_lod_levels[i], camera_position);
            float new_benefit = CalculateBenefit(objects_temp[i], new_level, camera_position);

            /* Calculate the benefit delta new_benefit/new_lod_cost - old_benefit/old_lod_cost */
            float benefit_delta = (new_benefit / objects_temp[i]->GetLodMesh(new_level)->GetNumTriangles() - old_benefit / objects_temp[i]->GetLodMesh(new_lod_levels[i])->GetNumTriangles());
            if (benefit_delta > best_benefit_delta) {
                /* If it larger than the previous benefit delta, keep it to be upgraded */
                best_benefit_delta = benefit_delta;
                object_to_upgrade = i;
            }
        }

        /* If none, then exit */
        if (object_to_upgrade == -1) break;

        /* Get the number of triangles in the previous level for the object to be upgraded */
        size_t prev_lod_triangles = objects_temp[object_to_upgrade]->GetLodMesh(new_lod_levels[object_to_upgrade])->GetNumTriangles();
        GraphicsObject::LodLevel new_level = static_cast<GraphicsObject::LodLevel>(new_lod_levels[object_to_upgrade] - 1);
        /* Calculate the new number of triangles, subtract the triangles from the previous frame! */
        current_triangles = current_triangles - prev_lod_triangles + objects_temp[object_to_upgrade]->GetLodMesh(new_level)->GetNumTriangles();
        
        /* If it's within range, set the change, else exit */
        if (current_triangles < max_triangles_per_frame) {
            new_lod_levels[object_to_upgrade] = new_level;
            last_frame_triangles_rendered_ = current_triangles;
        } else {
            break;
        }
    }

    /* Set the new lods. The call SetLod() also updates the time of change */
    for (size_t i = 0; i < n_objects; i++) {
        if (new_lod_levels[i] != objects_temp[i]->lod_chosen_level_) objects_temp[i]->SetLod(new_lod_levels[i]);
    }

}

void TimeCriticalRendering::ComputeTPS(size_t triangles_rendered, double frame_time) {

    /**
        Count the number of triangles rendered in the last frame, if we reach one second of rendering time,
        update the TPS with an average of 5
    */

    static double start;
    static unsigned long long triangles;

    start += frame_time;
    triangles += triangles_rendered;
    if (start >= 1.0) {

        TPSs_[TPS_index_ % 5] = triangles;
        TPS_ = (TPSs_[0] + TPSs_[1] + TPSs_[2] + TPSs_[3] + TPSs_[4]) / 5;

        TPS_index_++;
        start = 0;
        triangles = 0;
    }
}

TimeCriticalRendering::~TimeCriticalRendering() {

}

float TimeCriticalRendering::CalculateBenefit(GraphicsObject * object, GraphicsObject::LodLevel lod, glm::vec3 camera_position) {
    /* Function that calculates the benefit for each object */

    glm::vec3 mesh_position = glm::vec3(object->translate_[3][0], object->translate_[3][1], object->translate_[3][2]);
    GLfloat D = glm::distance(mesh_position, camera_position);

    GLfloat d = object->GetLodMesh(lod)->GetBoundingBox().GetDiagonalDistance();

    GLfloat twoL = std::pow(2, object->GetLodMesh(lod)->GetLodLevel());
    return (twoL * D) / d;

}
