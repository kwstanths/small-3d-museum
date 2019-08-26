#ifndef __VertexClustering_hpp__
#define __VertexClustering_hpp__

#include <deque>
#include <vector>

#include "TriangleMesh.h"
#include "Plane.hpp"

class VertexClustering {
public:
    enum Metric {
        MEAN,
        QUADRIC_ERROR
    };

    struct VertexCluster {
        std::deque<int> vertices_;
    };

public:
    VertexClustering();
    ~VertexClustering();

    void AddCluster(VertexCluster c);

    TriangleMesh * ClusterVertices(size_t lod_level, TriangleMesh * mesh, Metric metric, float cell_size);

private:
    std::vector<VertexCluster> clusters_;
};

#endif