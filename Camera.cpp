#include "Camera.h"
//#include <glm/gtx/string_cast.hpp>

Camera::Camera(int width, int height, glm::vec3 position)
{
	this->width = width;
	this->height = height;
	this->Position = position;
	this->speed = defaultSpeed;
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

	this->cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	GLint getUniform = glGetUniformLocation(shader.ID, uniform);
	if(getUniform != -1)
		glUniformMatrix4fv(getUniform, 1, GL_FALSE, glm::value_ptr(this->cameraMatrix));
	else
	{
		// Inactive shader uniform variable also returns -1
		// Ref: https://stackoverflow.com/questions/47871575/glgetuniformlocation-returns-1-for-samplers-other-than-first
		std::cout << "Camera Shader Error: " << __FILE__ << " line " << __LINE__ << ": GLSL uniform variable " << uniform << " not active!" << std::endl;
		exit(1);
		//std::cin.get();
	}
		
	// std::cout << glm::to_string(cameraMatrix) << std::endl;
}

// Controls camera movement
void Camera::Inputs(GLFWwindow* window)
{
	// Forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		this->Position += this->speed * this->Orientation;
	}
	// Left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		this->Position += this->speed * (-glm::normalize(glm::cross(this->Orientation, this->Up)));
	}
	// Backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		this->Position += this->speed * (-this->Orientation);
	}
	// Right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		this->Position += this->speed * (glm::normalize(glm::cross(this->Orientation, this->Up)));
	}
	// Up
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		this->Position += this->speed * this->Up;
	}
	// Down
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		this->Position += this->speed * (-this->Up);
	}

	// Speed Control
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		this->speed = defaultSpeed * 4;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		this->speed = this->defaultSpeed;
	}

	// Rotate with mouse
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (this->firstClick)
		{
			glfwSetCursorPos(window, (this->width / 2), (this->height / 2));
			this->firstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = this->sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = this->sensitivity * (float)(mouseX - (height / 2)) / height;
		
		glm::vec3 newOrientation = glm::rotate(this->Orientation, glm::radians(-rotX), glm::normalize(glm::cross(this->Orientation, this->Up)));
		if (!((glm::angle(newOrientation, this->Up)) <= glm::radians(5.0f) || (glm::angle(newOrientation, -this->Up) <= glm::radians(5.0f))))
		{
			this->Orientation = newOrientation;
		}

		this->Orientation = glm::rotate(this->Orientation, glm::radians(-rotY), this->Up);

		glfwSetCursorPos(window, (this->width / 2), (this->height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		this->firstClick = true;
	}
}