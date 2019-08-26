#include "Octree.hpp"

#include <iostream>
#include <algorithm>

Octree::Octree(glm::vec3 origin, float length) {     
    root_ = new OctreeInnerNode(origin, length);
}

Octree::~Octree() {
    delete root_;
}

void Octree::Insert(int vertex_id, glm::vec3 vertex) {
    root_->Insert(vertex_id, vertex);
}

size_t Octree::Depth() {
    return root_->Depth();
}

void Octree::ClusterNodes(size_t depth, VertexClustering * clusterer) {
    root_->ClusterNodes(depth, 0, clusterer);
}

Octree::OctreeInnerNode::OctreeInnerNode(glm::vec3 origin, float length) {
    children_ = std::vector<OctreeNode *>(8, nullptr);

    origin_ = origin;
    length_ = length;

    type_ = NodeType::INNER;
}

Octree::OctreeInnerNode::~OctreeInnerNode() {
    for (size_t i = 0; i < children_.size(); i++) {
        delete children_[i];
    }
}

void Octree::OctreeInnerNode::Insert(int vertex_id, glm::vec3 vertex) {

    /* Find child to insert to, index and new origin */
    size_t index = 0;
    
    float H = length_ / 2.0f;
    glm::vec3 new_node_origin = origin_;
    {
        glm::vec3 center = origin_ + glm::vec3(H, H, H);
        glm::vec3 V = vertex - center;
        if (V.x >= 0) {
            index += 1;
            new_node_origin.x += H;
        }
        if (V.y >= 0) {
            index += 2;
            new_node_origin.y += H;
        }
        if (V.z >= 0) {
            index += 4;
            new_node_origin.z += H;
        }
    }
    /* If it's null, create new leaf node */
    if (children_[index] == nullptr) {
        children_[index] = new OctreeLeafNode(vertex_id, vertex);
        return;
    }

    /* If it's inner node, insert to node */
    if (children_[index]->GetNodeType() == NodeType::INNER) {
        children_[index]->Insert(vertex_id, vertex);
        return;
    }
    /* If it's leaf node, get the data of the leaf node */
    int leaf_vertex_id = children_[index]->GetVertexID();
    glm::vec3 leaf_vertex = children_[index]->GetVertex();
    /* delete that leaf node */
    delete children_[index];

    /* Create inner node, and insert both vertices */
    children_[index] = new OctreeInnerNode(new_node_origin, H);
    children_[index]->Insert(leaf_vertex_id, leaf_vertex);
    children_[index]->Insert(vertex_id, vertex);
}

int Octree::OctreeInnerNode::GetVertexID() {
    /* Should never be called, INNER nodes dont hold data */
    return -1;
}

glm::vec3 Octree::OctreeInnerNode::GetVertex() {
    /* Should never be called, INNER nodes dont hold data */
    return glm::vec3(0.0f, 0.0f, 0.0f);
}

size_t Octree::OctreeInnerNode::Depth() {
    size_t current_depth = 0;
    for (size_t i = 0; i < children_.size(); i++)
        if (children_[i] != nullptr) current_depth = std::max(current_depth, children_[i]->Depth());

    return current_depth + 1;
}

void Octree::OctreeInnerNode::ClusterNodes(size_t depth, size_t current_depth, VertexClustering * clusterer) {
    /* If depth not reached, propagate the call */
    if (current_depth < depth) {
        for (size_t i = 0; i < children_.size(); i++)
            if (children_[i] != nullptr) children_[i]->ClusterNodes(depth, current_depth + 1, clusterer);
        return;
    }
    /* If node is reached, create a cluster and add leaf nodes to this cluster */
    VertexClustering::VertexCluster cluster;
    for (size_t i = 0; i < children_.size(); i++) {
        if (children_[i] != nullptr) children_[i]->AddNodesToCluster(cluster);
    }

    /* Add cluster to clusers */
    clusterer->AddCluster(cluster);
}

void Octree::OctreeInnerNode::AddNodesToCluster(VertexClustering::VertexCluster & cluster) {
    /* Propagate the call to the children */
    size_t number_of_children = 0;
    for (size_t i = 0; i < children_.size(); i++)
        if (children_[i] != nullptr) children_[i]->AddNodesToCluster(cluster);
}

Octree::OctreeLeafNode::OctreeLeafNode(int vertex_id, glm::vec3 vertex) {
    vertex_id_ = vertex_id;
    vertex_ = vertex;
    type_ = NodeType::LEAF;
}

void Octree::OctreeLeafNode::Insert(int vertex_id, glm::vec3 vertex) {
    std::cout << "OctreeLeafNode::Insert() Should never be called on a leaf node" << std::endl;
}

int Octree::OctreeLeafNode::GetVertexID() {
    return vertex_id_;
}

glm::vec3 Octree::OctreeLeafNode::GetVertex() {
    return vertex_;
}

size_t Octree::OctreeLeafNode::Depth() {
    return 0;
}

void Octree::OctreeLeafNode::ClusterNodes(size_t depth, size_t current_depth, VertexClustering * clusterer) {
    /* If this is called on a leaf node, create a cluster with a single vertex, and it to the clusters */
    VertexClustering::VertexCluster cluster;
    cluster.vertices_.push_back(vertex_id_);
    clusterer->AddCluster(cluster);
}

void Octree::OctreeLeafNode::AddNodesToCluster(VertexClustering::VertexCluster & cluster) {
    /* Add this leaf's vertex to the cluster */
    cluster.vertices_.push_back(vertex_id_);
}

Octree::OctreeNode::NodeType Octree::OctreeNode::GetNodeType() {
    return type_;
}
