#include "GraphicsObject.hpp"

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ConfigurationFile.hpp"

GraphicsObject::GraphicsObject(TriangleMesh * mesh_original, TriangleMesh * mesh_lod1, TriangleMesh * mesh_lod2, TriangleMesh * mesh_lod3) {
    
    lods_ = std::vector<TriangleMesh *>(4);
    lods_[0] = mesh_original;
    lods_[1] = mesh_lod1;
    lods_[2] = mesh_lod2;
    lods_[3] = mesh_lod3;

    /* If LOD not set, the original will be used */
    lod_chosen_level_ = LodLevel::ORIGINAL;
    last_lod_transition_timestamp_ = -10000;

    scale_ = glm::mat4(1.0f);
    translate_ = glm::mat4(1.0f);
}

GraphicsObject::~GraphicsObject() {
    
}

void GraphicsObject::SetPosition(glm::vec3 position) {
    translate_ = glm::translate(glm::mat4(1.0f), position);
}

void GraphicsObject::SetScale(glm::vec3 scale) {
    scale_ = glm::scale(glm::mat4(1.0f), scale);
}

glm::mat4 GraphicsObject::GetModel() {
    return translate_ * scale_;
}

GraphicsObject::LodLevel GraphicsObject::GetChosenLodLevel() {
    return lod_chosen_level_;
}

TriangleMesh * GraphicsObject::GetChosenLodMesh() {
    return lods_[lod_chosen_level_];
}

TriangleMesh * GraphicsObject::GetLodMesh(LodLevel level) {
    return lods_[level];
}

void GraphicsObject::SetLod(LodLevel level) {
    lod_chosen_level_ = level;
    last_lod_transition_timestamp_ = glutGet(GLUT_ELAPSED_TIME);
}

bool GraphicsObject::CanChangeLod() {
   
    return (glutGet(GLUT_ELAPSED_TIME) - last_lod_transition_timestamp_ > ConfigurationFile::instance().GetTCRTime());
}
