#include "Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>


Engine::Shader::Shader(const char * vertexShaderPath, const char * fragmentShaderPath)
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
	ID = glCreateProgram();
	glAttachShader(ID, vertexShaderId);
	glAttachShader(ID, fragmentShaderId);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete vertex and fragment shaders
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

Engine::Shader::~Shader() {
	glDeleteProgram(ID);
}

void Engine::Shader::Use()
{
	glUseProgram(ID);
}

void Engine::Shader::SetBool(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Engine::Shader::SetInt(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Engine::Shader::SetFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
