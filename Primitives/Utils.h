#ifndef UTILS_CLASS_H
#define UTILS_CLASS_H
#include <vector>
#include <iostream>
#include <algorithm>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <fstream>

class Utils
{


    public:

    static constexpr float EPSILON = 0.0000001f;
    static void showFloatVector(const std::vector<float>& v, const int breakInt = 1)
    {
        std::cout << "Float Vector: " << std::endl;
        for(int i = 0; i < v.size(); ++i)
        { 
            char br = (i % (breakInt) == 0) ? '\n' : '\t';
            std::cout << br << v[i]; 
        }
        std::cout << '\n';
    
    }

    static void showIntVector(const std::vector<GLuint>& v, const int breakInt = 1)
    {
        std::cout << "Int Vector: " << std::endl;
        for(int i = 0; i < v.size(); ++i)
        { 
            char br = (i % (breakInt) == 0) ? '\n' : '\t';
            std::cout << br << v[i]; 
        }
        std::cout << '\n';
    
    }

    static void showIntVector(const std::vector<int>& v, const int breakInt = 1)
    {
        std::cout << "Int Vector: " << std::endl;
        for(int i = 0; i < v.size(); ++i)
        { 
            char br = (i % (breakInt) == 0) ? '\n' : '\t';
            std::cout << br << v[i]; 
        }
        std::cout << '\n';
    
    }

    static void showGlmVec3(const glm::vec3& v)
    {
        std::cout << "GLM Vector: " << std::endl;
        std::cout << "X: " << v.x << " Y: " << v.y << " Z: " << v.z;
        std::cout << '\n';
    }

    static void showVec3Array(const std::vector<glm::vec3>& v)
    {
        std::cout << "Vector Array: " << std::endl;
        for(int i = 0; i < v.size(); ++i)
        {
            showGlmVec3(v[i]);
        }
        std::cout << '\n';
    }

    static void writeToFile(const char* path, const std::vector<glm::vec3>& v)
    {
        std::ofstream myfile;
        myfile.open (path);
        myfile << "Name: " << path << "\n\n";
        myfile << "\nVector Array: " << std::endl;
        for(int i = 0; i < v.size(); ++i)
        {
            if(i % 3 == 0)
                myfile << '\n';
           //std::cout << "\nGLM Vector: " << std::endl;
            myfile << "X: " << v[i].x << " Y: " << v[i].y << " Z: " << v[i].z;
            myfile << '\n';
        
        }
        
        myfile.close();
    }

    static void writeToFile(const char* path, const std::vector<int>& v, const int breakInt = 1)
    {
        std::ofstream myfile;
        myfile.open (path);

       myfile << "Name: " << path << "\n\n";
        myfile << "\nInt Vector: " << std::endl;
        for(int i = 0; i < v.size(); ++i)
        { 
            char br = (i % (breakInt) == 0) ? '\n' : '\t';
            myfile << br << v[i]; 
        }
        myfile << '\n';
         myfile.close();
    
    }

    static void writeToFile(const char* path, const char* s)
    {
        std::ofstream myfile;
        myfile.open (path);
        myfile << s;
        myfile.close();

    }

    static float map(float currentVal, float prevMin, float prevMax, float newMin, float newMax)
    {
        return (
            newMin + (newMax - newMin) / (prevMax - prevMin) * (currentVal - prevMin)
        );
    }

};

#endif // !UTILS_CLASS_H