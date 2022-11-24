#include "Mesh.h"

// TO DO: Create material class to pass instead of a texture
Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures)
{
	this->triangleCount = indices.size() / 3;
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	matrix = glm::mat4(1.0f);
	translation = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);

	vao.Bind();

	VBO vbo(vertices);
	EBO ebo(indices);

	vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.LinkAttribute(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao.LinkAttribute(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	vao.LinkAttribute(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();

}

void Mesh::DrawInstance
(
	Shader& shader, 
	Camera& camera,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale,
	bool isWire
)
{
	shader.Activate();
	vao.Bind();

	unsigned int numDiffuse = 0;
	unsigned int numSpec = 0;

	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		std::string num;
		std::string type = textures[i].type;

		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpec++);
		}
		else
		{
			std::cout << "Undefined Texture Type!\n\n";
		}

		textures[i].TexUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}

	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");

	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	// Use the line below to see the wireframe
	if(isWire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}


void Mesh::Draw
(
	Shader& shader, 
	Camera& camera,
	bool isWire
)
{
	this->DrawInstance(shader, camera, this->matrix, this->translation, this->rotation, this->scale, isWire);
}

std::vector<glm::vec3> Mesh::GetTransformedCoordinates(int triangleIndex) const
{
    if(triangleIndex < 0 || triangleIndex >= this->triangleCount)
	{
		std::cout << "Mesh Error: " << __FILE__ << " line " << __LINE__ << ": Invalid triangle index :" << triangleIndex << std::endl;
		return std::vector<glm::vec3> { glm::vec3(0,0,0) };
	}
        

    std::vector<glm::vec3> triVerts;
    int offset = triangleIndex * 3;
    for(int i = 0; i < 3; ++i)
    {
		// For each vertex apply transformation
		// resultpoint = matrix * translation * rotation * scale * attributevertex
		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);
		trans = glm::translate(trans, translation);
		rot = glm::mat4_cast(rotation);
		sca = glm::scale(sca, scale);
		glm::vec3 aVert = vertices[indices[offset + i]].position;
		glm::vec4 hVerts = glm::vec4(aVert.x, aVert.y, aVert.z, 1);

		hVerts = matrix * trans * rot * sca * hVerts;
		// Push the result vertex into a vector
        triVerts.push_back(glm::vec3(hVerts));
    }
    return triVerts;
}

std::vector<glm::vec3> Mesh::GetTransformedTriangles() const
{
	// std::vector<glm::vec3> vertices =
	// {
	// 	glm::vec3(-1, -1, -1),
	// 	glm::vec3(-2, -3, 3),
	// 	glm::vec3(-3, 3, 1),
	// 	glm::vec3(1, 2, 2),
	// 	glm::vec3(2, 4, 2)
	// };

	std::vector<glm::vec3> vertices;
	//std::cout << "Mesh has triangles: " << this->triangleCount << std::endl;
	for(int i = 0; i < this->triangleCount; ++i)
	{
		std::vector<glm::vec3> triangle = this->GetTransformedCoordinates(i);
		for(int j = 0; j < triangle.size(); ++j)
		{

			vertices.push_back(triangle[j]);
		}
	}
	return vertices;
}

glm::vec3 Mesh::GetTriangleCentroid(int triangleIndex) const
{
	std::vector<glm::vec3> triangle = this->GetTransformedCoordinates(triangleIndex);
	glm::vec3 centroid = triangle[0];
	for(int j = 1; j < triangle.size(); ++j)
	{
		centroid += triangle[j];
	}
	centroid.x /= 3.0f;
	centroid.y /= 3.0f;
	centroid.z /= 3.0f;
	return centroid;
}
