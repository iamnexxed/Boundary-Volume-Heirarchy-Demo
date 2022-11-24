#include "ShaderClass.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		// std::cout << contents << std::endl;
		return (contents);
	}
	else
	{
		std::cout << "Could not read file: " << filename << std::endl;
	}
	throw (errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX", vertexFile);


	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT", fragmentFile);

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertexShader);
	glAttachShader(this->ID, fragmentShader);

	glLinkProgram(this->ID);
	compileErrors(this->ID, "PROGRAM", "LINK");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(this->ID);
}

void Shader::Delete()
{
	glDeleteProgram(this->ID);
}


void Shader::compileErrors(unsigned int shader, const char* type, const char* name)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (std::strcmp(type,  "PROGRAM") != 0)
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << __FILE__ << ": line " << __LINE__ << ":SHADER_COMPILATION_ERROR for: " << type << " file: " << name << "\n\n"
					  << infoLog << "\n\n";
			return;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			GLsizei length;
			glGetProgramInfoLog(shader, 1024, &length, infoLog);
			std::cout <<__FILE__ << ": line " << __LINE__ << ": SHADER_LINKING_ERROR for: " << type << "\n\n" << length << " " << infoLog << "\n\n";
			return;
		}
	}

	if (hasCompiled == GL_TRUE)
	{
		std::cout << name << " has compiled successfully. \n\n";
	}
}