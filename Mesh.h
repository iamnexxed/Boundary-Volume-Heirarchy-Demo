#ifndef HEAD_CLASS_H
#define HEAD_CLASS_H

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class Mesh
{
public:

	int triangleCount;

	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	glm::mat4 matrix;
	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
	VAO vao;

	Mesh
	(
		std::vector <Vertex>& vertices, 
		std::vector <GLuint>& indices, 
		std::vector <Texture>& textures
	);

	void Draw
	(
		Shader& shader, 
		Camera& camera,
		bool isWire = false
	);

	void DrawInstance
	(
		Shader& shader, 
		Camera& camera,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
		bool isWire = false
	);

	std::vector<glm::vec3> GetTransformedCoordinates(int triangleIndex) const;
	std::vector<glm::vec3> GetTransformedTriangles() const;

	glm::vec3 GetTriangleCentroid(int triangleIndex) const;

};

#endif // !HEAD_CLASS_H
