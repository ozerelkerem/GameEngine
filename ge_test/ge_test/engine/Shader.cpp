#pragma once

#include "Shader.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../includes/GL/glew.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vertexFileStream, fragmentFileStream;
	std::stringstream vertexBuffer, fragmentBuffer;
	try
	{
		vertexFileStream.open(vertexPath);
		fragmentFileStream.open(fragmentPath);

		

		vertexBuffer << vertexFileStream.rdbuf();
		fragmentBuffer << fragmentFileStream.rdbuf();

		vertexFileStream.close();
		fragmentFileStream.close();
	}
	catch (const std::exception&)
	{
		throw std::exception("Cannot Read From ShaderFile");
	}
	
	
	const char * fragmentCode = fragmentBuffer.str().c_str();
	const char * vertexCode = vertexBuffer.str().c_str();
	
	glewInit();


	unsigned int vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);

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