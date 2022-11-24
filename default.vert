
#version 330 core

layout (location = 0) in vec3 aPos;

layout (location = 1) in vec3 aNormal;

layout (location = 2) in vec3 aColor;

layout (location = 3) in vec2 aTex;


out vec3 currentPos;
out vec3 normal;
out vec3 color;

out vec2 texCoord;

// Transformation inputs
uniform mat4 camMatrix;
// Imports model matrix 
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;



void main()
{
	currentPos = vec3(model * translation * (-rotation) * scale * vec4(aPos, 1.0f));

	normal = aNormal;
	
	//gl_Position = camMatrix * vec4(aPos, 1.0);
	color = aColor;

	// Rotation transformation
	//texCoord = mat2(0.0f, 1.0f, -1.0f, 0.0f) * aTex;
	texCoord = aTex;

	// multiply by vec4(aPos, 1.0); for default settings
	gl_Position =  camMatrix * vec4(currentPos, 1.0);
}