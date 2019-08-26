#include "VertexClustering.hpp"

#include <iostream>
#include <unordered_map>

VertexClustering::VertexClustering() {
}

VertexClustering::~VertexClustering() {
}

void VertexClustering::AddCluster(VertexCluster c) {
    clusters_.push_back(c);
}

TriangleMesh * VertexClustering::ClusterVertices(size_t lod_level, TriangleMesh * mesh, Metric metric, float cell_size) {

    /* Grab the necessary data */
    std::vector<glm::vec3> & vertices = mesh->vertices;
    std::vector<int> & triangles = mesh->triangles;
    std::vector<std::deque<int>> & faces_per_vertex = mesh->faces_per_vertex_;
    std::vector<Plane> & triangle_planes = mesh->triangle_planes_;

    /* Create a new mesh, pass the lod_level */
    TriangleMesh * new_mesh = new TriangleMesh(lod_level);

    /* Holds the mapping between old to new vertices */
    std::unordered_map<int, int> old_vertices_to_new_vertices;

    size_t number_of_means = 0;
    size_t number_of_qems = 0;
    int new_vertex_id = 0;
    /* For all clusters */
    for (size_t i = 0; i < clusters_.size(); i++) {
        VertexCluster c = clusters_[i];

        /* if cluster empty, continue. This shouldn't happen, but oh well...*/
        if (c.vertices_.size() == 0) continue;

        if (metric == Metric::MEAN) {
            glm::vec3 mean = glm::vec3(0, 0, 0);
            /* Iterate through the vertices in this cluster */
            for (std::deque<int>::iterator v_id = c.vertices_.begin(); v_id != c.vertices_.end(); ++v_id) {\
                /* Add to mean */
                mean += vertices[(*v_id)];
                /* Set the mapping, *itr is the index of the old vertex */
                old_vertices_to_new_vertices[*v_id] = new_vertex_id;
            }

            /* Add it to the mesh */
            mean.x = mean.x / c.vertices_.size();
            mean.y = mean.y / c.vertices_.size();
            mean.z = mean.z / c.vertices_.size();
            new_mesh->addVertex(mean);
            number_of_means++;
            new_vertex_id++;
            
        } else {
            /* Accumulate mean and Quadric error matrix*/
            glm::dmat4 Qp = glm::dmat4(0.0f);
            /* Also calculate mean */
            glm::vec3 mean(0.0f);

            for (std::deque<int>::iterator v_id = c.vertices_.begin(); v_id != c.vertices_.end(); ++v_id) {
                /* Add to mean */
                mean += vertices[(*v_id)];
                /* *itr is the index of the old vertex */
                old_vertices_to_new_vertices[*v_id] = new_vertex_id;
                /* For all neighbouring triangles of that vertex in the cluster, accumulate triangles Kp to total Qp */
                for (std::deque<int>::iterator face_id = faces_per_vertex[*v_id].begin(); face_id != faces_per_vertex[*v_id].end(); ++face_id) {
                    Qp += triangle_planes[*face_id].Kp_;
                }
            }

            /* Calcualte final QEM matrix */
            glm::dmat4 QEM;
            QEM[0][0] = Qp[0][0];
            QEM[0][1] = Qp[0][1];
            QEM[0][2] = Qp[0][2];
            QEM[0][3] = Qp[0][3];
            QEM[1][0] = Qp[1][0];
            QEM[1][1] = Qp[1][1];
            QEM[1][2] = Qp[1][2];
            QEM[1][3] = Qp[1][3];
            QEM[2][0] = Qp[2][0];
            QEM[2][1] = Qp[2][1];
            QEM[2][2] = Qp[2][2];
            QEM[2][3] = Qp[2][3];
            QEM[3][0] = 0;
            QEM[3][1] = 0;
            QEM[3][2] = 0;
            QEM[3][3] = 1;

            /* Calcualte the mean */
            mean.x = mean.x / c.vertices_.size();
            mean.y = mean.y / c.vertices_.size();
            mean.z = mean.z / c.vertices_.size();

            /* Calculate the determinant, check if it's close enough to zero */
            double det = glm::determinant(QEM);
            if (!Equal(det, 0.0, 0.00001)) {
                /* If not, solve the equation */
                glm::vec3 p = glm::vec4(0, 0, 0, 1) * glm::inverse(QEM);

                /**
                    Check if it's outside the cell, this is a rough approximation, but it's very easy to calculate, and it works...
                */
                if (glm::distance(p, mean) > cell_size) {
                    new_mesh->addVertex(mean);
                    number_of_means++;
                } else {
                    /* If survived all checks, add the qem */
                    new_mesh->addVertex(p);
                    number_of_qems++;
                }

            } else {
                /* If close enough to zero, add the mean */
                new_mesh->addVertex(mean);
                number_of_means++;
            }
            new_vertex_id++;
        }
    }

    /* Create new mesh */
    /* Traverse all old triangles */
    size_t new_triangles = 0;
    for (size_t i = 0; i < triangles.size(); i += 3) {
        /* Find the old vertices that formed this triangle */
        int old_vertex_v0 = triangles[i];
        int old_vertex_v1 = triangles[i + 1];
        int old_vertex_v2 = triangles[i + 2];

        /* Find the new vertex represenative for each old vertex */
        int new_vertex_v0 = old_vertices_to_new_vertices[old_vertex_v0];
        int new_vertex_v1 = old_vertices_to_new_vertices[old_vertex_v1];
        int new_vertex_v2 = old_vertices_to_new_vertices[old_vertex_v2];

        /* if the all three of the old vertices fell into a different cell, we have a proper triangle */
        if (new_vertex_v0 != new_vertex_v1 && new_vertex_v1 != new_vertex_v2 && new_vertex_v0 != new_vertex_v2) {
            new_mesh->addTriangle(new_vertex_v0, new_vertex_v1, new_vertex_v2);
            new_triangles++;
        }
    }
    std::cout << "Vertex clustering: \n\tOld triangles: " << triangles.size() / 3 << ", New triangles: " << new_triangles << std::endl;
    std::cout << "\tQEMs: " << number_of_qems << ", Means: " << number_of_means << std::endl;
    new_mesh->ComputeAddInfo();

    return new_mesh;
}
