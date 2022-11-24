#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <json/json.h>
#include "Mesh.h"

using json = nlohmann::json;

class Model
{
private:
	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	

	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;

	void LoadMesh(unsigned int indMesh);

	void TraverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	std::vector <unsigned char> GetData();
	std::vector <float> GetFloats(json accessor);
	std::vector<GLuint> GetIndices(json accessor);
	std::vector<Texture> GetTextures();

	std::vector<Vertex> AssembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	std::vector<glm::vec2> GroupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> GroupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> GroupFloatsVec4(std::vector<float> floatVec);

public:

	std::vector<Mesh> meshes;

	glm::vec3 defaultColor;

	// TO DO: Create a parent transform matrix and pass it to the TraverseNode in the constructor

	Model(const char* file, glm::vec3 defaultColor = glm::vec3(1.0f, 1.0f, 1.0f));

	void Draw
	(
		Shader& shader,
		Camera& camera
	);

	
};

#endif // !MODEL_CLASS_H

