#ifndef _TRIANGLE_MESH_INCLUDE
#define _TRIANGLE_MESH_INCLUDE

#include <string>
#include <vector>
#include <deque>

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Plane.hpp"
#include "AABox.hpp"

using namespace std;


// Class TriangleMesh renders a very simple room with textures


template<typename T> static T map_to_range(T in_a, T in_b, T in_t, T out_a, T out_b) {
    return out_a + ((out_b - out_a) / (in_b - in_a)) * (in_t - in_a);
}

template<typename T> static bool Equal(const T& a, const T& b, T e = std::numeric_limits<T>::epsilon()) {
    return std::abs(a - b) <= e;
}

class TriangleMesh
{
    friend class VertexClustering;
public:
    enum Metric {
        MEAN,
        QUADRIC_ERROR
    };
    /**
        @param lod_level The level of detail of this mesh, in terms of log2 of the clustering grid size, 0 means origina mesh
    */
	TriangleMesh(size_t lod_level);

	void addVertex(const glm::vec3 &position);
	void addTriangle(int v0, int v1, int v2);

	void buildCube();
    void buildTile();
	
	void sendToOpenGL(ShaderProgram &program);
	void render() const;
    void renderPoints() const;
	void free();

    void ComputeAddInfo();

    TriangleMesh * VertexClusteringGrid(size_t grid_size,  Metric metric);

    std::vector<TriangleMesh *> VertexClusteringOctree(std::vector<size_t> depths, Metric metric);

    size_t GetNumTriangles();

    size_t GetLodLevel();

    AABox GetBoundingBox();

private:
    vector<glm::vec3> vertices;
    vector<int> triangles;
    vector<Plane> triangle_planes_;
    vector<std::deque<int>> faces_per_vertex_;
    AABox bounding_box_;

    /* Holds the lod level in terms of log2 of the grid clustering grid size */
    size_t lod_level_;

	GLuint vao;
	GLuint vbo;
	GLint posLocation, normalLocation;
	
};


#endif // _TRIANGLE_MESH_INCLUDE
