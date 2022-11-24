#ifndef BOID_CLASS_H
#define BOID_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include "Model.h"
#include <cmath>
#include "Primitives/Utils.h"
#include "Primitives/Mathematics.h"


class Boid
{
    private:
    glm::vec3 position;
    glm::vec3 velocity;
    float centerDistance;
    const float steeringIntensity = 0.01;
    float wanderAngle;
    glm::vec3 currentTarget;

    int frameCounter = 0;

    public:
    Boid(glm::vec3 startPosition);
    void AddAcceleration(glm::vec3 acc);
    void Seek(glm::vec3 target);
    void SphericalWander(float sphereRadius = 1);
    void Update();
    glm::vec3 GetPosition();
    void SetSpherePosition(glm::vec3 vel);
    void Draw(Shader& shader, Camera& camera, Mesh& mesh);

};



#endif 
