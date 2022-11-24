#ifndef BVH_CLASS_H
#define BVH_CLASS_H

#include <vector>
#include <glm/glm.hpp>
#include <algorithm>    // std::min_element, std::max_element
#include <stack>
#include "Box.h"
#include "Primitives/Utils.h"
#include "Model.h"
#include "Physics/physics.h"



class BVH;
class BVHNode
{
    private:

    static int idCounter;

    int id;
    Box* boundingBox;
    std::vector<int> triangleIndices;
    BVHNode* lChild;
    BVHNode* rChild;
    int depth;
    friend class BVH;
    BVHNode(const glm::vec3& vmin, const glm::vec3& vmax, int depth);
    void drawDebugBox
    (
        Shader& shader, 
		Camera& camera, 
        int depthCheck
    );
    void drawBoxIndex
    (
        Shader& shader, 
		Camera& camera, 
        int index
    );
};

class BVH
{
    private:
    BVHNode* root;

    

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> centroids;
    const int shapesPerBox = 2;
    glm::vec3 findMinVertex(std::vector<glm::vec3>& triVertices);
    glm::vec3 findMaxVertex(std::vector<glm::vec3>& triVertices);
    std::vector<int> GetTriIndicesWithinBounds(const Mesh& mesh, const glm::vec3& vmin, const glm::vec3& vmax);
    void SortTrianglesOnAxis(std::vector<int>& triangleIndices, const int axisIndex);
    void TraverseChildren(BVHNode* next);
    bool isVisited(const std::vector<int>& v, int value);
    
    public:
    
    

    BVH(Model& model);
    BVH(Mesh& mesh);
    void createTree(const Model& model);
    void createTree(const Mesh& mesh);
    //bool findIntersectionPoint(glm::vec3& origin, glm::vec3 dir, glm::vec3& outPoint);

    bool findIntersectionPoint(Shader& shader, Camera& camera, glm::vec3 origin, glm::vec3 dir, glm::vec3& outPoint);
    void showBoxes
    (
        Shader& shader, 
		Camera& camera,
        int maxDepth
    );
    void showBoxIndex
    (
        Shader& shader, 
		Camera& camera,
        int index
    );
    std::vector<int> GetIndicesOnRight(const std::vector<int>& indices, bool isRight);
    std::vector<glm::vec3> GetTriangleVerticesFromIndices(const std::vector<int>& indices);
};



#endif // !BVH_CLASS_H