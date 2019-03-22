#pragma once

#include "Shader.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include "../ogl/GL/glew.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vertexFileStream, fragmentFileStream;
	std::stringstream vertexBuffer, fragmentBuffer;
	std::string s_vertexCode, s_fragmentCode;
	try
	{
		vertexFileStream.open(vertexPath);
		fragmentFileStream.open(fragmentPath);

		

		vertexBuffer << vertexFileStream.rdbuf();
		fragmentBuffer << fragmentFileStream.rdbuf();


		s_vertexCode = vertexBuffer.str();
		s_fragmentCode = fragmentBuffer.str();

		vertexFileStream.close();
		fragmentFileStream.close();
	}
	catch (const std::exception&)
	{
		throw std::exception("Cannot Read From ShaderFile");
	}
	
	
	const char * fragmentCode = s_fragmentCode.c_str();
	const char * vertexCode = s_vertexCode.c_str();
	
	

	unsigned int vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);


	char infoLog[1024];
	glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
	if (infoLog[0])
		throw std::exception("Shader Error");
	glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
	if (infoLog[0])
		throw std::exception("Shader Error");

	programID = glCreateProgram();
	glAttachShader(programID, vertex);
	glAttachShader(programID, fragment);
	glLinkProgram(programID);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use()
{
	glUseProgram(programID);

}

void Shader::setFloat(const std::string &name, const float &val)
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), val);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &v3)
{
	glUniform3f(glGetUniformLocation(programID, name.c_str()), v3[0], v3[1] , v3[2]);
}
void Shader::setVec3(const std::string &name, const glm::tvec3<uint32_t>&v3)
{
	glUniform3f(glGetUniformLocation(programID, name.c_str()), v3[0], v3[1], v3[2]);
}
void Shader::setVec3(const std::string &name, const float v1, const float v2, const float v3)
{
	glUniform3f(glGetUniformLocation(programID, name.c_str()), v1, v2, v3);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &m4)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &m4[0][0]);
}

void Shader::setMat4Array(const std::string &name, size_t size, const glm::mat4 &m4)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), size, GL_FALSE, glm::value_ptr(m4));
}

void Shader::setInt(const std::string &name, const int &val)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), val);
}


unsigned int Shader::getProgramID() { return programID; }