#include "TriangleMesh.h"

#include <iostream>
#include <algorithm>
#include <deque>
#include <vector>
#include <unordered_map>
#include <ctime>

#include "UniformGrid.hpp"
#include "Octree.hpp"
#include "VertexClustering.hpp"

using namespace std;


TriangleMesh::TriangleMesh(size_t lod_level) {
    /* Each triangle mesh stores the lod level that it represents */
    /* zero means original mesh */
    lod_level_ = lod_level;
}

void TriangleMesh::addVertex(const glm::vec3 &position)
{
  vertices.push_back(position);
}

void TriangleMesh::addTriangle(int v0, int v1, int v2)
{
  triangles.push_back(v0);
  triangles.push_back(v1);
  triangles.push_back(v2);
}

void TriangleMesh::buildCube() {

    /* cube's half size */
    float hs = 0.5;
	float vertices[] = {-hs, -hs, -hs,
                      hs, -hs, -hs,
                      hs,  hs, -hs,
                      -hs,  hs, -hs,
                      -hs, -hs,  hs,
                      hs, -hs,  hs,
                      hs,  hs,  hs,
                      -hs,  hs,  hs };

	int faces[] = {3, 1, 0, 3, 2, 1,
                 5, 6, 7, 4, 5, 7,
                 7, 3, 0, 0, 4, 7,
                 1, 2, 6, 6, 5, 1,
                 0, 1, 4, 5, 4, 1,
                 2, 3, 7, 7, 6, 2 };

	int i;

	for(i=0; i<8; i+=1)
		addVertex(glm::vec3(vertices[3*i], vertices[3*i+1], vertices[3*i+2]));
	for(i=0; i<12; i++)
		addTriangle(faces[3*i], faces[3*i+1], faces[3*i+2]);
}

void TriangleMesh::buildTile() {
    float vertices[] = { -1, 0, -1,
                      1, 0, -1,
                      1,  0, 1,
                      -1,  0, 1
    };

    int faces[] = { 0, 2, 1, 0, 3, 2 };

    int i;

    for (i = 0; i < 4; i += 1)
        addVertex(0.5f * glm::vec3(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2]));
    for (i = 0; i < 2; i++)
        addTriangle(faces[3 * i], faces[3 * i + 1], faces[3 * i + 2]);
}

void TriangleMesh::sendToOpenGL(ShaderProgram &program) {
	vector<float> data;

	for(unsigned int tri=0; tri<triangles.size(); tri+=3) {
        glm::vec3 normal = triangle_planes_[tri / 3].normal_;
        for(unsigned int vrtx=0; vrtx<3; vrtx++) {
            data.push_back(vertices[triangles[tri + vrtx]].x);
            data.push_back(vertices[triangles[tri + vrtx]].y);
            data.push_back(vertices[triangles[tri + vrtx]].z);
            
            data.push_back(normal.x);
            data.push_back(normal.y);
            data.push_back(normal.z);
        }
    }

    // Send data to OpenGL
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    posLocation = program.bindVertexAttribute("position", 3, 6 * sizeof(float), 0);
    normalLocation = program.bindVertexAttribute("normal", 3, 6 * sizeof(float), (void *)(3 * sizeof(float)));
}

void TriangleMesh::render() const
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(normalLocation);
	glDrawArrays(GL_TRIANGLES, 0, triangles.size());
}

void TriangleMesh::renderPoints() const {

    glBindVertexArray(vao);
    glEnableVertexAttribArray(posLocation);
    glEnableVertexAttribArray(normalLocation);
    glDrawArrays(GL_POINTS, 0, vertices.size());
}

void TriangleMesh::free()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	
	vertices.clear();
	triangles.clear();
}

void TriangleMesh::ComputeAddInfo() {
    
    /* Calculate faces per vertex array */
    faces_per_vertex_ = std::vector<std::deque<int> >(vertices.size());
    for (int i = 0; i < triangles.size(); i += 3) {
        int v0 = triangles[i];
        int v1 = triangles[i + 1];
        int v2 = triangles[i + 2];

        faces_per_vertex_[v0].push_back(i / 3);
        faces_per_vertex_[v1].push_back(i / 3);
        faces_per_vertex_[v2].push_back(i / 3);
    }

    /* Calcuate the planes per triangle array */
    triangle_planes_ = std::vector<Plane>(triangles.size() / 3);
    for (int i = 0; i < triangles.size(); i += 3) {
        int v0 = triangles[i];
        int v1 = triangles[i + 1];
        int v2 = triangles[i + 2];
        triangle_planes_[i / 3] = Plane(vertices[v0], vertices[v1], vertices[v2]);
    }

    /* Calculate the bounding box */
    GLfloat min_x, min_y, min_z, max_x, max_y, max_z;
    min_x = max_x = vertices[0].x;
    min_y = max_y = vertices[0].y;
    min_z = max_z = vertices[0].z;
    for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i].x < min_x) min_x = vertices[i].x;
        if (vertices[i].x > max_x) max_x = vertices[i].x;
        if (vertices[i].y < min_y) min_y = vertices[i].y;
        if (vertices[i].y > max_y) max_y = vertices[i].y;
        if (vertices[i].z < min_z) min_z = vertices[i].z;
        if (vertices[i].z > max_z) max_z = vertices[i].z;
    }
    bounding_box_ = AABox(glm::vec3(min_x, min_y, min_z), glm::vec3(max_x, max_y, max_z));
    
}

TriangleMesh * TriangleMesh::VertexClusteringGrid(size_t grid_size, Metric metric) {

    /* grid_end holds the end of the grid as a floating point number */
    float grid_end = 1.0f * grid_size;
    /* 3D uniform grid of size +1, each cell holds a cluster of vertices */
    UniformGrid<VertexClustering::VertexCluster, 3> grid(grid_size + 1);

    /* Traverse old vertices, find where they belong */
    for (size_t i = 0; i < vertices.size(); i++) {
        size_t grid_i = static_cast<size_t>(map_to_range(bounding_box_.min_.x, bounding_box_.max_.x, vertices[i].x, 0.0f, grid_end));
        size_t grid_j = static_cast<size_t>(map_to_range(bounding_box_.min_.y, bounding_box_.max_.y, vertices[i].y, 0.0f, grid_end));
        size_t grid_k = static_cast<size_t>(map_to_range(bounding_box_.min_.z, bounding_box_.max_.z, vertices[i].z, 0.0f, grid_end));

        /* Add to the cluster at the cell */
        grid.at(grid_i, grid_j, grid_k).vertices_.push_back(i);
    }

    /* Add clusters to the VertexClustering object */
    VertexClustering * clusterer = new VertexClustering();
    for (size_t i = 0; i < grid_size + 1; i++) {
        for (size_t j = 0; j < grid_size + 1; j++) {
            for (size_t k = 0; k < grid_size + 1; k++) {
                if (grid.at(i, j, k).vertices_.size() != 0) clusterer->AddCluster(grid.at(i, j, k));
            }
        }
    }

    /* Calculate cell_size. It will be used for QEM */
    float cell_size = ((float)grid_size) / std::max(std::max(bounding_box_.max_.x - bounding_box_.max_.x, bounding_box_.max_.y - bounding_box_.max_.y), bounding_box_.max_.z - bounding_box_.max_.z);
    /* Perform Vertex clustering based on the chosen metric */
    return clusterer->ClusterVertices(log(grid_size), this, static_cast<VertexClustering::Metric>(metric), cell_size);
}

std::vector<TriangleMesh *> TriangleMesh::VertexClusteringOctree(std::vector<size_t> depths, Metric metric) {
    
    std::vector<TriangleMesh *> meshes = std::vector<TriangleMesh *>(depths.size(), nullptr);

    /* Create a single octree, based on the size of the bounding box  */
    float length = std::max(std::max(bounding_box_.max_.x, bounding_box_.max_.y), bounding_box_.max_.z) - std::min(std::min(bounding_box_.min_.x, bounding_box_.min_.y), bounding_box_.min_.z);
    Octree octree(glm::vec3(bounding_box_.min_.x, bounding_box_.min_.y, bounding_box_.min_.z), length);
    
    std::clock_t begin = std::clock();

    /* Insert all vertices */
    for (size_t i = 0; i < vertices.size(); i++)
        octree.Insert(i, vertices[i]);

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Insertion time: " << elapsed_secs << std::endl;

    /* Create one mesh for each depth */
    for (size_t i = 0; i < depths.size(); i++) {

        size_t octree_depth = depths[i];

        std::clock_t begin = std::clock();

        VertexClustering * clusterer = new VertexClustering();
        octree.ClusterNodes(octree_depth, clusterer);

        meshes[i] = clusterer->ClusterVertices(octree_depth, this, static_cast<VertexClustering::Metric>(metric), length);

        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        std::cout << "Clustering time: " << elapsed_secs << std::endl;
    }

    return meshes;
}

size_t TriangleMesh::GetNumTriangles() {
    return triangles.size() / 3;
}

size_t TriangleMesh::GetLodLevel() {
    return lod_level_;
}

AABox TriangleMesh::GetBoundingBox() {
    return bounding_box_;
}



