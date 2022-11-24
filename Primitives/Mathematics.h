#ifndef MATHEMATICS_CLASS_H
#define MATHEMATICS_CLASS_H

#include <glm/glm.hpp>
#include <stdlib.h>
#include <time.h>
#include <cmath>



class Mathematics
{
    public:

    static glm::vec3 GetRandomPointOnSphere(float radius)
    {
        int lon, lat;
        

        lat = rand() % 180;
        lon = rand() % 360;
        // Reference: https://stackoverflow.com/questions/39937628/howto-use-glm-constants-in-c-opengl
        float theta = lat * glm::pi<float>() / 180.0f;
        float phi = lon * glm::pi<float>() / 180.0f;
        float x = radius * std::sin(theta) * std::cos(phi);
        float y = radius * std::sin(theta) * std::sin(phi);
        float z = radius * std::cos(theta);

        return glm::vec3(x, y, z);
    }
};



#endif 
