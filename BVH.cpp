#include "BVH.h"

int BVHNode::idCounter = 0;

BVHNode::BVHNode(const glm::vec3& vmin, const glm::vec3& vmax, int depth)
{
   glm::vec3 eVec = glm::vec3(Utils::EPSILON, Utils::EPSILON, Utils::EPSILON);
   this->boundingBox = new Box(vmin - eVec, vmax + eVec, glm::vec3(0, 0, 1));
   //this->boundingBox = new Box(vmin, vmax, glm::vec3(0, 0, 1));

   this->lChild = nullptr;
   this->rChild = nullptr;
   this->depth = depth;
   this->id = BVHNode::idCounter++;
}

void BVHNode::drawDebugBox(Shader& shader, Camera& camera, int depthCheck = -1)
{
   
   
   if(depthCheck == this->depth)
   {
      this->boundingBox->DrawWireFrame(shader, camera);

   }

      if(lChild != nullptr)
         this->lChild->drawDebugBox(shader, camera, depthCheck);

      if(rChild != nullptr)
         this->rChild->drawDebugBox(shader, camera, depthCheck);
  

   
}

void BVHNode::drawBoxIndex
(
   Shader& shader, 
   Camera& camera, 
   int index
)
{
    if(index == this->id)
   {
      this->boundingBox->DrawWireFrame(shader, camera);
      return;
   }

   if(lChild != nullptr)
      this->lChild->drawBoxIndex(shader, camera, index);

   if(rChild != nullptr)
      this->rChild->drawBoxIndex(shader, camera, index);
}



BVH::BVH(Model& model)
{

   this->root = nullptr;
   this->createTree(model);
}

BVH::BVH(Mesh& mesh)
{
   this->root = nullptr;
   this->createTree(mesh);
}

glm::vec3 BVH::findMinVertex(std::vector<glm::vec3>& triVertices)
{
    glm::vec3 minx = *std::min_element(triVertices.begin(), triVertices.end(), [&](glm::vec3 a, glm::vec3 b) { return a.x < b.x; });
    glm::vec3 miny = *std::min_element(triVertices.begin(), triVertices.end(), [&](glm::vec3 a, glm::vec3 b) { return a.y < b.y; });
    glm::vec3 minz = *std::min_element(triVertices.begin(), triVertices.end(), [&](glm::vec3 a, glm::vec3 b) { return a.z < b.z; });
    return glm::vec3(minx.x, miny.y, minz.z);
}

glm::vec3 BVH::findMaxVertex(std::vector<glm::vec3>& triVertices)
{
    glm::vec3  maxx = *std::max_element(triVertices.begin(), triVertices.end(), [&](glm::vec3 a, glm::vec3 b) { return a.x < b.x; });
    glm::vec3  maxy = *std::max_element(triVertices.begin(), triVertices.end(), [&](glm::vec3 a, glm::vec3 b) { return a.y < b.y; });
    glm::vec3  maxz = *std::max_element(triVertices.begin(), triVertices.end(), [&](glm::vec3 a, glm::vec3 b) { return a.z < b.z; });
    return glm::vec3(maxx.x, maxy.y, maxz.z);
}

void BVH::createTree(const Mesh& mesh)
{
   this->vertices = mesh.GetTransformedTriangles();

   for(int i = 0; i < mesh.triangleCount; ++i)
   {
      this->centroids.push_back(mesh.GetTriangleCentroid(i));
   }

   // A) Find the min and max of x, y and z axes respectively
   glm::vec3 minV = this->findMinVertex(this->vertices);
   glm::vec3 maxV = this->findMaxVertex(this->vertices);

   //std::cout << "\nMin:";
   //Utils::showGlmVec3(minV);

   //std::cout << "\nMax:";
   //Utils::showGlmVec3(maxV);

   // B) create a boundingbox with min x,y,z and the max x,y,z
   // C) Store as the root node
   this->root = new BVHNode(minV, maxV, 0);
   this->root->triangleIndices = this->GetTriIndicesWithinBounds(mesh, minV, maxV);



   this->TraverseChildren(root);

//    // D) Find the longest axis and sort the centroid of the triangles on that axis

   
//   this->SortTrianglesOnAxis(this->root->triangleIndices,this->root->boundingBox->longestAxisIndex);

//    // E) Find the midpoint on the longest axis


//    // F) Split the triangles into two sections. Find children nodes

//    // G) For both the sections create a new node and a bounding box and attach the node to the parent



   // H) Repeat steps D to G till the node bounding box contains max 2 triangles
}

// Reference: https://www.haroldserrano.com/blog/visualizing-the-boundary-volume-hierarchy-collision-algorithm
void BVH::createTree(const Model& model)
{
   this->createTree(model.meshes[0]);
   
}

// Reference: https://www.cs.toronto.edu/~heap/270F02/node36.html
bool BVH::findIntersectionPoint(Shader& shader, Camera& camera, glm::vec3 origin, glm::vec3 dir, glm::vec3& outPoint)
{

   glm::vec3 boxnPoint;
	glm::vec3 boxfPoint;
   glm::vec3 iPoint;

   std::stack<BVHNode*> nodeStack;
   std::vector<int> visitedIds;
   std::vector<glm::vec3> iPoints;
   // Find the intersection of the ray starting from root
   // Set current to root
   

      //std::cout << "Ray Intersects root." << std::endl;
      // if(this->root->lChild->boundingBox->IntersectsRay(origin, dir, 0, 100, boxnPoint, boxfPoint))
      // {
      //    std::cout << "Current Depth: " << this->root->lChild->depth << std::endl;
      // }

      //Utils::showGlmVec3(this->root->lChild->boundingBox->bounds[0]);
      //Utils::showGlmVec3(this->root->lChild->boundingBox->bounds[1]);


   BVHNode* current = this->root;
   nodeStack.push(current);

   //return false;
   while(!nodeStack.empty())
   {
      current = nodeStack.top();
      nodeStack.pop();

      if(current->triangleIndices.size() <= this->shapesPerBox)
      {
         
         //current->boundingBox->DrawWireFrame(shader, camera);
         for(int i = 0; i < current->triangleIndices.size(); ++i)
         {
            std::vector<int> indices = { current->triangleIndices[i] };
            std::vector<glm::vec3> triCoord = this->GetTriangleVerticesFromIndices(indices);
            if(Physics::RayIntersectsTriangle(origin, dir, triCoord, outPoint))
            {
               //return true;
               iPoints.push_back(outPoint);
            }
            //std::cout << "Correct Triangle not found for: " << current->triangleIndices[i] << std::endl;
            //std::cout << "Triangle vertices: ";
            //Utils::showVec3Array(triCoord);
         }
         
         
      } 
      else
      {

         // Check if ray intersects with current
         if(current->lChild != nullptr && !this->isVisited(visitedIds, current->lChild->id) && current->lChild->boundingBox->IntersectsRay(origin, dir, 0, 100, boxnPoint, boxfPoint))
         {
            //std::cout << "\n\nHas LChild\n";
           
            //current = current->lChild;
            //std::cout << "Current Depth: " << current->depth << std::endl;
            visitedIds.push_back(current->lChild->id);
            nodeStack.push(current->lChild);
            
         }
         

         if(current->rChild != nullptr && !this->isVisited(visitedIds, current->rChild->id) && current->rChild->boundingBox->IntersectsRay(origin, dir, 0, 100, boxnPoint, boxfPoint))
         {
            //std::cout << "\n\nHas RChild\n";
            //std::cout << "Current Depth: " << current->depth << std::endl;
            visitedIds.push_back(current->rChild->id);
            nodeStack.push(current->rChild);
         }      
      }
      
   }
   
   if(iPoints.size() > 0)
   {
      int minIndex = 0;
      float min = glm::distance(origin, iPoints[minIndex]);
      //std::cout << "I Points: " << iPoints.size() << std::endl;
      for(int i = 1; i < iPoints.size(); ++i)
      {
         float d = glm::distance(origin, iPoints[i]);
         if(d < min)
         {
            min = d;
            minIndex = i;
         }
      }
      outPoint = iPoints[minIndex];
      return true;
   }
   return false;
}

void BVH::showBoxes
(
   Shader& shader, 
   Camera& camera,
   int maxDepth
)
{
   this->root->drawDebugBox(shader, camera, maxDepth);
}

void BVH::showBoxIndex
(
   Shader& shader, 
   Camera& camera,
   int index
)
{
   this->root->drawBoxIndex(shader, camera, index);
}

// TO DO: Refactor this
std::vector<int> BVH::GetTriIndicesWithinBounds(const Mesh& mesh, const glm::vec3& vmin, const glm::vec3& vmax)
{
   
   int triCount = mesh.triangleCount;
   std::vector<int> indices;
   for(int i = 0; i < triCount; ++i)
   {
      const std::vector<glm::vec3> verts = mesh.GetTransformedCoordinates(i);
      int k = 0;
      for(int j = 0; j < verts.size(); ++j)
      {
         if
         (
            verts[j].x >= vmin.x &&
            verts[j].y >= vmin.y && 
            verts[j].z >= vmin.z &&
            verts[j].x <= vmax.x &&
            verts[j].y <= vmax.y &&
            verts[j].z <= vmax.z
         )
         {
            k++;
         }
         else
         {
            break;
         }
         
      }
      if(k == verts.size())
         indices.push_back(i);
   }

   return indices;
}


void BVH::SortTrianglesOnAxis(std::vector<int>& triangleIndices, const int axisIndex)
{
   switch(axisIndex)
   {
      case 0: // X Axis
         std::sort(triangleIndices.begin(), triangleIndices.end(), [&](int i, int j) { return this->centroids[i].x < this->centroids[j].x; });
      break;

      case 1: // Y Axis
         std::sort(triangleIndices.begin(), triangleIndices.end(), [&](int i, int j) { return this->centroids[i].y < this->centroids[j].y; });

      break;

      case 2: // Z Axis
         std::sort(triangleIndices.begin(), triangleIndices.end(), [&](int i, int j) { return this->centroids[i].z < this->centroids[j].z; });

      break;

      default:
               std::cout << "BVH Error: " << __FILE__ << " line " << __LINE__ << ": Invalid axis index :" << axisIndex << std::endl;
   }
}

std::vector<int> BVH::GetIndicesOnRight(const std::vector<int>& indices, bool isRight)
{
   std::vector<int> partIndices;

  
   
      if(!isRight)
      {
         for(int i = 0; i < indices.size() / 2; ++i)
         {
            partIndices.push_back(indices[i]);
         }
      }
      else
      {
         for(int i = indices.size() / 2; i < indices.size(); ++i)
         {
            partIndices.push_back(indices[i]);
         }
      }
   

   return partIndices;
}

std::vector<glm::vec3> BVH::GetTriangleVerticesFromIndices(const std::vector<int>& indices)
{
   std::vector<glm::vec3> vertices;
   for(int i = 0; i < indices.size(); ++i)
   {
      
      for(int j = 0; j < 3; ++j)
      {
         vertices.push_back(this->vertices[indices[i] * 3 + j]);
      }

   }

   return vertices;
}

void BVH::TraverseChildren(BVHNode* next)
{
   //std::cout << "Depth: " << next->depth << std::endl;
   
   //std::cout << "Next ID: " << next->id << std::endl;
   // D) Find the longest axis and sort the centroid of the triangles on that axis

   //std::cout << "Centroid within root bounds: " << this->centroids.size() << std::endl;
   //std::cout << "Longest Axis: " << root->boundingBox->longestAxisIndex << std::endl;
   
  this->SortTrianglesOnAxis(next->triangleIndices, next->boundingBox->longestAxisIndex);

   // E) Find the midpoint on the longest axis
   glm::vec3 midpoint = next->boundingBox->GetLongestAxisMid();
 

   // F) Split the triangles into two sections. 
   std::vector<int> child1Indices = this->GetIndicesOnRight(next->triangleIndices, false);
   std::vector<int> child2Indices = this->GetIndicesOnRight(next->triangleIndices, true);

   //Utils::writeToFile("DebugFiles/childIndices1.txt", child1Indices);
   //Utils::writeToFile("DebugFiles/childIndices2.txt", child2Indices);

   // std::cout << "\nChild1 Indices: " << child1Indices.size();
   // std::cout << "\nChild2 Indices: " << child2Indices.size();
   // std::cout << "\nLongest axis: " << next->boundingBox->longestAxisIndex;
   // std::cout << "\nMidpoint at: \n";
   // Utils::showGlmVec3(midpoint);
   // std::cout << std::endl << std::endl;

   if(child1Indices.size() > 0)
   {
      std::vector<glm::vec3> tris1 = this->GetTriangleVerticesFromIndices(child1Indices);
      //Utils::writeToFile("DebugFiles/childtris1.txt", tris1);
  

      glm::vec3 child1minV = this->findMinVertex(tris1);
      glm::vec3 child1maxV = this->findMaxVertex(tris1);
      BVHNode* c1Node = new BVHNode(child1minV, child1maxV, next->depth + 1);
      c1Node->triangleIndices = child1Indices;
      next->lChild = c1Node;
      if(child1Indices.size() > this->shapesPerBox)
         this->TraverseChildren(next->lChild);


   }

   if(child2Indices.size() > 0)
   {
      std::vector<glm::vec3> tris2 = this->GetTriangleVerticesFromIndices(child2Indices);
      //Utils::writeToFile("DebugFiles/childtris2.txt", tris2);
      glm::vec3 child2minV = this->findMinVertex(tris2);
      glm::vec3 child2maxV = this->findMaxVertex(tris2);
      BVHNode* c2Node = new BVHNode(child2minV, child2maxV, next->depth + 1);
      c2Node->triangleIndices = child2Indices;
      next->rChild = c2Node;
      if(child2Indices.size() > this->shapesPerBox)
         this->TraverseChildren(next->rChild);

   }
 


   // G) For both the sections create a new node and a bounding box and attach the node to the parent
   //Utils::writeToFile("DebugFiles/childtris1.txt", tris1);
   //Utils::writeToFile("DebugFiles/childtris2.txt", tris2);

   //std::cout << "\nChild1 Tris: " << tris1.size();
   //std::cout << "\nChild2 Tris: " << tris2.size();


  
   //std::cout << "\nChild 1 at:";
   //Utils::showGlmVec3(child1minV);
   //Utils::showGlmVec3(child1maxV);

   //std::cout << "\nChild 2 at:";
   //Utils::showGlmVec3(child2minV);
   //Utils::showGlmVec3(child2maxV);

   // H) Repeat steps D to G till the node bounding box contains max 2 triangles
}


bool BVH::isVisited(const std::vector<int>& v, int value)
{
   for(int i = 0; i < v.size(); ++i)
   {
      if(v[i] == value)
         return true;
   }

   return false;
}