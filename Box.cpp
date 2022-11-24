#include "Box.h"

Box::Box(const glm::vec3& vmin, const glm::vec3& vmax, const glm::vec3& color)
{
    if(!this->isValidBox(vmin, vmax))
    {
        std::cerr << "\nBox cannot be constructed!\n";
    }

    // Box Reference Image: https://drive.google.com/file/d/194uSVnU0UEA9hNuHMCDKfeAuBKr91dLX/view?usp=sharing

    this->bounds.push_back(vmin);
    this->bounds.push_back(vmax);
    this->color = color;
    this->lines.resize(12);
    this->longestAxisIndex = -1;
    for(int i = 0; i < this->lines.size(); ++i)
    {
        this->lines[i].setColor(color);
    }
    this->CalculateLines();
}

bool Box::IntersectsRay
(
    const glm::vec3& origin, 
    const glm::vec3& direction, 
    float t0,
    float t1,
    glm::vec3& outNPoint,
    glm::vec3& outFPoint
)
{
    glm::vec3 inv_direction = glm::vec3(1 / direction.x, 1 / direction.y, 1 / direction.z);
    int sign[3];
    sign[0] = (inv_direction.x < 0);
    sign[1] = (inv_direction.y < 0);
    sign[2] = (inv_direction.z < 0);

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (this->bounds[sign[0]].x - origin.x) * inv_direction.x;
    tmax = (this->bounds[1 - sign[0]].x - origin.x) * inv_direction.x;
    tymin = (this->bounds[sign[1]].y - origin.y) * inv_direction.y;
    tymax = (this->bounds[1 - sign[1]].y - origin.y) * inv_direction.y;

    if((tmin > tymax) || (tymin > tmax))
        return false;

    if(tymin > tmin)
        tmin = tymin;
    
    if(tymax < tmax)
        tmax = tymax;

    tzmin = (this->bounds[sign[2]].z - origin.z) * inv_direction.z;
    tzmax = (this->bounds[1 - sign[2]].z - origin.z) * inv_direction.z;

    if(tmin > tzmax || tzmin > tmax)
        return false;

    if(tzmin > tmin)
        tmin = tzmin;
    if(tzmax < tmax)
        tmax = tzmax;

    if((tmin < t1) && (tmax > t0))
    {
        outNPoint = origin + tmin * direction;
        outFPoint = origin + tmax * direction;
        return true;
    }
    return false;
}


void Box::CalculateLines()
{
    glm::vec3 v0 = glm::vec3(bounds[0].x, bounds[1].y, bounds[0].z);
    glm::vec3 v1 = glm::vec3(bounds[0].x, bounds[1].y, bounds[1].z);
    glm::vec3 v2 = glm::vec3(bounds[0].x, bounds[0].y, bounds[1].z);
    glm::vec3 v3 = glm::vec3(bounds[1].x, bounds[1].y, bounds[0].z);
    glm::vec3 v4 = glm::vec3(bounds[1].x, bounds[0].y, bounds[0].z);
    glm::vec3 v5 = glm::vec3(bounds[1].x, bounds[0].y, bounds[1].z);

    this->lines[0].setPoints(bounds[0], v0);
    this->lines[1].setPoints(v0, v3);
    this->lines[2].setPoints(v3, v4);
    this->lines[3].setPoints(bounds[0], v4);

    this->lines[4].setPoints(v1, bounds[1]);
    this->lines[5].setPoints(bounds[1], v5);
    this->lines[6].setPoints(v5, v2);
    this->lines[7].setPoints(v2, v1);

    this->lines[8].setPoints(bounds[0], v2);
    this->lines[9].setPoints(v0, v1);
    this->lines[10].setPoints(v3, bounds[1]);
    this->lines[11].setPoints(v4, v5);

    this->longestAxisIndex = this->GetLongestAxis();
}

void Box::DrawWireFrame(Shader& shader, Camera& camera)
{
    for(int i = 0; i < this->lines.size(); ++i)
    {
        this->lines[i].Draw(shader, camera);
    }
}

int Box::GetLongestAxis()
{
    std::vector<float> xyzLen = 
    {
        // TO DO: Refactor to store the right, up and forward axes neatly
        this->lines[3].getLength(), // Right Axis
        this->lines[0].getLength(), // Up Axis
        this->lines[8].getLength()  // Forward Axis
    };
    
    float max = xyzLen[0];
    //float max = -1;
    int axisIndex = 0;
    for(int i = 1; i < xyzLen.size(); ++i)
    {
        if(xyzLen[i] > max) 
        {
            max = xyzLen[i];
            axisIndex = i;
        }
    }
    return axisIndex; // TO DO: Create a Global variable enum that contains the axis index

}


glm::vec3 Box::GetLongestAxisMid()
{
    switch(this->longestAxisIndex)
    {
        case 0:
            return this->lines[3].getMidPoint();
        break;
        case 1:
            return this->lines[0].getMidPoint();
        break;
        case 2:
            return this->lines[8].getMidPoint();
        break;

        default: 
            std::cout << "Invalid Longest axis mid point\n";

    }
    return glm::vec3(0, 0, 0);
}

bool Box::isValidBox(const glm::vec3& vmin, const glm::vec3& vmax)
{
    return
    (
        vmin.x < vmax.x && 
        vmin.y < vmax.y && 
        vmin.z < vmax.z 
    );
}