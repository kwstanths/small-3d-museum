#ifndef __GraphicsObject_hpp__
#define __GraphicsObject_hpp__

#include <vector>

#include <glm/glm.hpp>

#include "TriangleMesh.h"

static glm::vec4 LodColors[4] = { glm::vec4(0.9f, 0.9f, 0.95f, 1.0f), glm::vec4(1,0,0,1), glm::vec4(0,1,0,1), glm::vec4(0,0,1,1) };

class GraphicsObject {
    friend class TimeCriticalRendering;
public:

    enum LodLevel
    {
        ORIGINAL = 0,
        ONE = 1,
        TWO = 2,
        THREE = 3,
    };

    GraphicsObject(TriangleMesh * mesh_original, TriangleMesh * mesh_lod1, TriangleMesh * mesh_lod2, TriangleMesh * mesh_lod3);
    ~GraphicsObject();

    void SetPosition(glm::vec3 position);

    void SetScale(glm::vec3 scale);

    glm::mat4 GetModel();

    LodLevel GetChosenLodLevel();

    TriangleMesh * GetChosenLodMesh();

    TriangleMesh * GetLodMesh(LodLevel level);

    void SetLod(LodLevel level);

    bool CanChangeLod();

private:
    glm::mat4 translate_;
    glm::mat4 scale_;
    LodLevel lod_chosen_level_;
    int last_lod_transition_timestamp_;

    /* Hold four triangle meshes in the respective positions */
    std::vector<TriangleMesh *> lods_;
};


#endif