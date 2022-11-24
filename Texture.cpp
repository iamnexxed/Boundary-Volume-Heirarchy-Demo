#include "Texture.h"

Texture::Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType)
{
	type = texType;

	int widthImg, heightImg, numColorChannels;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColorChannels, 0);

	
	glGenTextures(1, &ID);

	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// GL_LINEAR : returns the weighted average of the four texture elements that are closest to the center of the pixel being textured.
	// GL_NEAREST : returns the value of the texture element that is nearest (in Manhattan distance) to the center of the pixel being textured.
	// Reference Link : https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Show image with border color
	// float flatColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	// glTexParameterfv(textType, GL_TEXTURE_BORDER_COLOR, flatColor);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);

	// Generate smaller resolution versions
	// glGenerateMipmap(textType);

	// free image data as the data has been imported
	stbi_image_free(bytes);

	glBindTexture(GL_TEXTURE_2D, 0);

	
}

Texture::Texture(const char* image, const char* texType, GLuint slot)
{
	type = texType;

	int widthImg, heightImg, numColorChannels;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColorChannels, 0);


	glGenTextures(1, &ID);

	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// GL_LINEAR : returns the weighted average of the four texture elements that are closest to the center of the pixel being textured.
	// GL_NEAREST : returns the value of the texture element that is nearest (in Manhattan distance) to the center of the pixel being textured.
	// Reference Link : https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Show image with border color
	// float flatColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	// glTexParameterfv(textType, GL_TEXTURE_BORDER_COLOR, flatColor);

	if (numColorChannels == 4)
	{
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			bytes
		);
	}
	else if (numColorChannels == 3)
	{
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			bytes
		);
	}
	else if (numColorChannels == 1)
	{
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			bytes
		);
	}
	else
	{
		throw std::invalid_argument("Automatic Texture type recognition failed");
	}
	// Generate smaller resolution versions
	glGenerateMipmap(GL_TEXTURE_2D);

	// free image data as the data has been imported
	stbi_image_free(bytes);

	glBindTexture(GL_TEXTURE_2D, 0);


}

void Texture::TexUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}