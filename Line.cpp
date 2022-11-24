#include "Line.h"

Line::Line()
{
	glm::vec3 defaultVec = glm::vec3(1, 1, 1);
	this->createGLBuffer(defaultVec, defaultVec, defaultVec);
}

Line::Line(const glm::vec3& startPosition, const glm::vec3& endPosition, const glm::vec3& color)
{
	this->createGLBuffer(startPosition, endPosition, color);
}

void Line::setStartPosition(glm::vec3 newPosition)
{
	this->line_vertices[0].position = newPosition;
	this->vbo->SetVertex(0, line_vertices[0]);
}

glm::vec3 Line::getStartPosition()
{
	return this->line_vertices[0].position;
}

void Line::setEndPosition(glm::vec3 newPosition)
{
	this->line_vertices[1].position = newPosition;
	this->vbo->SetVertex(1, line_vertices[1]);
}

glm::vec3 Line::getEndPosition()
{
	return this->line_vertices[1].position;
}

void Line::setPoints(glm::vec3 startPosition,glm::vec3 endPosition)
{
	this->setStartPosition(startPosition);
	this->setEndPosition(endPosition);
}

// Reference: https://stackoverflow.com/questions/14486291/how-to-draw-line-in-opengl
void Line::Draw
(
	Shader& shader, 
	Camera& camera
)
{
	shader.Activate();
	this->vao.Bind();

	

	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");

	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml
	glDrawArrays(GL_LINE_STRIP, 0, 2);
}

void  Line::createGLBuffer
(	
	const glm::vec3& startPosition,
	const glm::vec3& endPosition,
	const glm::vec3& color
)
{
	this->vao.Bind();

										//COORDINATES				/			NORMALS				/			COLORS					/		TEXTURE COORDINATES
	this->line_vertices.push_back(Vertex{ startPosition,				glm::vec3(0.0f, 1.0f, 0.0f),				color,				glm::vec2(0.0f, 0.0f) });
	this->line_vertices.push_back(Vertex{ endPosition,				glm::vec3(0.0f, 1.0f, 0.0f),				color,				glm::vec2(0.0f, 1.0f) });


	this->vbo = new VBO(line_vertices, false);


	this->vao.LinkAttribute(*vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	this->vao.LinkAttribute(*vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	this->vao.LinkAttribute(*vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	//vao.LinkAttribute(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	
	this->vao.Unbind();
	this->vbo->Unbind();
}

void Line::setColor(glm::vec3 newColor)
{
	this->line_vertices[0].color = newColor;
	this->vbo->SetVertex(0, line_vertices[0]);

	this->line_vertices[1].color = newColor;
	this->vbo->SetVertex(1, line_vertices[1]);
}

float Line::getLength()
{
	glm::vec3 direction = this->line_vertices[1].position - this->line_vertices[0].position;
	return glm::length(direction);
}

glm::vec3 Line::getMidPoint()
{
	return glm::vec3
	(
		(this->line_vertices[0].position.x + this->line_vertices[1].position.x) / 2.0f,
		(this->line_vertices[0].position.y + this->line_vertices[1].position.y) / 2.0f,
		(this->line_vertices[0].position.z + this->line_vertices[1].position.z) / 2.0f
	);
}