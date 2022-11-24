#include "Boid.h"

Boid::Boid(glm::vec3 startPosition)
{
    this->position = startPosition;
    this->velocity = glm::vec3(1, 0, 0);
    this->wanderAngle = 0;
    this->centerDistance = glm::length(this->position);
    this->currentTarget = this->position;
}

void Boid::AddAcceleration(glm::vec3 acc)
{
    this->velocity += acc;
}

void Boid::Seek(glm::vec3 target)
{
    // Find the direction target - this->velocity;
    //
    this->AddAcceleration(target - this->velocity);
}

void Boid::SphericalWander(float sphereRadius)
{
    if(frameCounter >= 100)
    {
        this->currentTarget = Mathematics::GetRandomPointOnSphere(sphereRadius);
        frameCounter = 0;
    }
    
    this->Seek(this->currentTarget);
}

void Boid::Update()
{
    this->SphericalWander(this->centerDistance);
    //std::cout << "Wander Angle: " << this->wanderAngle << std::endl;
    this->SetSpherePosition(glm::normalize(this->velocity) * steeringIntensity);
    frameCounter++;
}

glm::vec3 Boid::GetPosition()
{
    return position;
}

void Boid::Draw(Shader& shader, Camera& camera, Mesh& mesh) 
{
    this->Update();
    //std::cout << "Boid at: ";
    //Utils::showGlmVec3(this->position);
    mesh.DrawInstance(shader, camera, mesh.matrix, this->position, mesh.rotation, mesh.scale);
}

void Boid::SetSpherePosition(glm::vec3 vel)
{
    this->position += vel;
    this->position = glm::normalize(this->position) * this->centerDistance;
}