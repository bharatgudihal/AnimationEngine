#include "Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>


Engine::Graphics::Shader::Shader(const char * vertexShaderPath, const char * fragmentShaderPath)
{
	std::string vertexShaderCode;
	std::string fragmentShaderCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	vertexShaderFile.open(vertexShaderPath);
	fragmentShaderFile.open(fragmentShaderPath);

	std::stringstream vertexShaderStream, fragmentShaderStream;
	vertexShaderStream << vertexShaderFile.rdbuf();
	fragmentShaderStream << fragmentShaderFile.rdbuf();

	vertexShaderFile.close();
	fragmentShaderFile.close();

	vertexShaderCode = vertexShaderStream.str();
	fragmentShaderCode = fragmentShaderStream.str();

	const char* vertexShader = vertexShaderCode.c_str();
	const char* fragmentShader = fragmentShaderCode.c_str();

	unsigned int vertexShaderId, fragmentShaderId;
	int success;
	char infoLog[512];

	// Create and compile vertex shader
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertexShader, NULL);
	glCompileShader(vertexShaderId);
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create and compile fragment shader
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragmentShader, NULL);
	glCompileShader(fragmentShaderId);
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create shader program
	shaderId = glCreateProgram();
	glAttachShader(shaderId, vertexShaderId);
	glAttachShader(shaderId, fragmentShaderId);
	glLinkProgram(shaderId);
	glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete vertex and fragment shaders
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

Engine::Graphics::Shader::~Shader() {
	glDeleteProgram(shaderId);
}

void Engine::Graphics::Shader::Use()
{
	glUseProgram(shaderId);
}

void Engine::Graphics::Shader::SetBool(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(shaderId, name.c_str()), static_cast<int>(value));
}

void Engine::Graphics::Shader::SetInt(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
}

void Engine::Graphics::Shader::SetFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
}

void Engine::Graphics::Shader::SetVector(const std::string & name, const glm::vec3 & vector)
{
	unsigned int location = glGetUniformLocation(shaderId, name.c_str());
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void Engine::Graphics::Shader::SetMatrix(const std::string & name, const glm::mat4& matrix) const
{
	unsigned int location = glGetUniformLocation(shaderId, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
