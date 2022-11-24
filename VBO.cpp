#include "VBO.h"


VBO::VBO(std::vector <Vertex>& vertices, bool isStatic)
{

	glGenBuffers(1, &ID);

	this->Bind();
	isStatic 
		? glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW)
		: glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STREAM_DRAW);
}

void VBO::SetVertices(std::vector<Vertex>& vertices)
{
	this->Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

}

void VBO::SetVertex(int index, Vertex& newVertex)
{
	this->Bind();
	glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(Vertex), sizeof(Vertex), &newVertex);
}


void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}