#ifndef PHYSICS_CLASS_H
#define PHYSICS_CLASS_H
#include <glm/glm.hpp>
#include <vector>
#include "../VBO.h"
#include "../Primitives/Utils.h"

namespace Physics
{
    // Reference: https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
    static bool RayIntersectsTriangle
    (
        const glm::vec3& origin, 
        const glm::vec3& direction, 
        const std::vector<glm::vec3>& triangleVertices, 
        glm::vec3& outPoint
    )
    {
        

        glm::vec3 v0 = triangleVertices[0];
        glm::vec3 v1 = triangleVertices[1];
        glm::vec3 v2 = triangleVertices[2];

        glm::vec3 edge1, edge2, h, s, q;

        float a, f, u, v;
        edge1 = v1 - v0;
        edge2 = v2 - v0;

        h = glm::cross(direction, edge2);

        a = glm::dot(edge1, h);

        if(a > -Utils::EPSILON && a < Utils::EPSILON)
            return false;

        f = 1.0 / a;

        s = origin - v0;
        u = f * glm::dot(s, h);

        if(u < 0.0f || u > 1.0f)
        return false;

        q = glm::cross(s, edge1);
        v = f * glm::dot(direction, q);

        if(v < 0.0f || u + v > 1.0f)
            return false;

        float t = f * glm::dot(edge2, q);

        if(t > Utils::EPSILON)
        {
            outPoint = origin + direction * t;
            return true;
        }

        // Line intersection but not a ray intersection
        return false;
    }

    // Reference: http://people.csail.mit.edu/amy/papers/box-jgt.pdf
    static bool RayIntersectsBox
    (
        const glm::vec3& origin, 
        const glm::vec3& direction, 
        const std::vector<glm::vec3>& boxBounds, 
        float t0,
        float t1
    )
    {
        glm::vec3 inv_direction = glm::vec3(1 / direction.x, 1 / direction.y, 1 / direction.z);
        int sign[3];
        sign[0] = (inv_direction.x < 0);
        sign[1] = (inv_direction.y < 0);
        sign[2] = (inv_direction.z < 0);

        float tmin, tmax, tymin, tymax, tzmin, tzmax;

        tmin = (boxBounds[sign[0]].x - origin.x) * inv_direction.x;
        tmax = (boxBounds[1 - sign[0]].x - origin.x) * inv_direction.x;
        tymin = (boxBounds[sign[1]].y - origin.y) * inv_direction.y;
        tymax = (boxBounds[1 - sign[1]].y - origin.y) * inv_direction.y;

        if((tmin > tymax) || (tymin > tmax))
            return false;

        if(tymin > tmin)
            tmin = tymin;
        
        if(tymax < tmax)
            tmax = tymax;

        tzmin = (boxBounds[sign[2]].z - origin.z) * inv_direction.z;
        tzmax = (boxBounds[1 - sign[2]].z - origin.z) * inv_direction.z;

        if(tmin > tzmax || tzmin < tmax)
            return false;

        if(tzmin > tmin)
            tmin = tzmin;
        if(tzmax < tmax)
            tmax = tzmax;

        return ((tmin < t1) && (tmax > t0));
    }
    
}

#endif // !PHYSICS_CLASS_H