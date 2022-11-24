#ifndef BOX_CLASS_H
#define BOX_CLASS_H

#include <glm/glm.hpp>
#include <vector>
#include "Line.h"
#include <algorithm>

// TO DO: Rename as AABBBox
class Box
{
    private:

    std::vector<Line> lines;
    //int longestLineIndex;

    public: 
    int longestAxisIndex;
    std::vector<glm::vec3> bounds;
    glm::vec3 color;

    Box(const glm::vec3& vmin, const glm::vec3& vmax, const glm::vec3& color = glm::vec3(1.0, 1.0, 1.0));
    bool IntersectsRay
    (
        const glm::vec3& origin, 
        const glm::vec3& direction, 
        float t0,
        float t1,
        glm::vec3& outNPoint,
        glm::vec3& outFPoint
    );
    bool isValidBox(const glm::vec3& vmin, const glm::vec3& vmax);
    void CalculateLines();
    void DrawWireFrame(Shader& shader, Camera& camera);
    int GetLongestAxis();
    glm::vec3 GetLongestAxisMid();
};


#endif // !LINE_CLASS_H