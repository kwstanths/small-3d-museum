#ifndef __Octree_hpp__
#define __Octree_hpp__

#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

#include "VertexClustering.hpp"

class Octree {
private:
    /**
        Generic tree node
        INNER: octree nodes that hold 8 pointers to children. They dont hold actual data
        LEAF: octree nodes that are at the final levels. The dont hold pointers. They hold data
    */
    class OctreeNode {
    public:
        enum NodeType {
            LEAF,
            INNER,
        };

        virtual ~OctreeNode() {

        }

        NodeType GetNodeType();

        /* Overrided by the INNER and LEAF implementations */
        virtual void Insert(int vertex_id, glm::vec3 vertex) = 0;
        virtual int GetVertexID() = 0;
        virtual glm::vec3 GetVertex() = 0;
        virtual size_t Depth() = 0;
        virtual void ClusterNodes(size_t depth, size_t current_depth, VertexClustering * clusterer) = 0;
        virtual void AddNodesToCluster(VertexClustering::VertexCluster & cluster) = 0;


    protected:
        NodeType type_;
    };

    /**
        A LEAF octree node
    */
    class OctreeLeafNode : public OctreeNode {
    public:
        OctreeLeafNode(int vertex_id, glm::vec3 vertex);

        /* Inserting to LEAF nodes doesn't make any sense, calling this, is a logical error */
        void Insert(int vertex_id, glm::vec3 vertex);

        int GetVertexID();
        glm::vec3 GetVertex();
        size_t Depth();
        void ClusterNodes(size_t depth, size_t current_depth, VertexClustering * clusterer);
        void AddNodesToCluster(VertexClustering::VertexCluster & cluster);

    private:
        /* Holds a vertex id, and the position of that vertex */
        int vertex_id_;
        glm::vec3 vertex_;
    };

    /**
        An INNER octree node
    */
    class OctreeInnerNode : public OctreeNode {
    public:
        OctreeInnerNode(glm::vec3 origin, float length);
        ~OctreeInnerNode();

        void Insert(int vertex_id, glm::vec3 vertex);
        int GetVertexID();
        glm::vec3 GetVertex();
        size_t Depth();
        void ClusterNodes(size_t depth, size_t current_depth, VertexClustering * clusterer);
        void AddNodesToCluster(VertexClustering::VertexCluster & cluster);

    private:
        /* Pointers to other nodes */
        std::vector<OctreeNode *> children_;
        /* The origin (bottom left) of this node's bounding box */
        glm::vec3 origin_;
        /* The length of this node's bounding box in all directions */
        float length_;
    };

public:
    /**
        @param origin The origin (bottom left) of the octree's bounding box
        @param length The length of the bounding box in all three directions. Please be positive
    */
    Octree(glm::vec3 origin, float length);

    ~Octree();

    /**
        Insert data in the tree
        @param vertex_id The id of the vertex
        @param vertex The position of the vertex
    */
    void Insert(int vertex_id, glm::vec3 vertex);

    /**
        Get the depth of the tree
    */
    size_t Depth();

    /**
        Perform clustering
        @param depth The maximum depth to reach for clustered nodes
        @param clusterer A VertexClustering object
    */
    void ClusterNodes(size_t depth, VertexClustering * clusterer);

private:
    OctreeInnerNode * root_;
};

#endif