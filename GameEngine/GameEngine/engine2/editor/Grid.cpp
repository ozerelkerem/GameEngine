#pragma once
#include "Grid.h"

#include "../ogl/GL/glew.h"

Grid::Grid(int lineCount, float spaceLength, unsigned int programID)
{
	this->spaceLength = spaceLength;
	this->lineCount = lineCount;

	this->programID = programID;
}

void Grid::Draw()
{
	
	lineCount += lineCount % 2;
	float limit = ((float)(spaceLength * lineCount) / 2);

	int idPosition = glGetAttribLocation(colorShader->programID, "position");



	colorShader->setInt("hasBones",0);
	colorShader->setVec3("color", gridColor[0], gridColor[1], gridColor[2]);
	glBegin(GL_LINES);
	for (float i = -limit; i < 0; i += spaceLength)
	{
		glVertexAttrib3f(idPosition, i, limit, 0);
		glVertexAttrib3f(idPosition, i, -limit, 0);
		glVertexAttrib3f(idPosition, limit, i, 0);
		glVertexAttrib3f(idPosition, -limit, i, 0);
	}
	glEnd();
	//x axis

	colorShader->setVec3("color", 1.0, 0, 0);
	glBegin(GL_LINES);
	glVertexAttrib3f(idPosition, limit, 0, 0);
	glVertexAttrib3f(idPosition, -limit, 0, 0);
	glEnd();
	//y axis
	colorShader->setVec3("color", 0, 1.0, 0);
	glBegin(GL_LINES);
	glVertexAttrib3f(idPosition, 0, limit, 0);
	glVertexAttrib3f(idPosition, 0, -limit, 0);
	glEnd();
	//z+
	colorShader->setVec3("color", 0, 0, 1.0);
	glBegin(GL_LINES);
	glVertexAttrib3f(idPosition, 0, 0, -limit);
	glVertexAttrib3f(idPosition, 0, 0, limit);
	glEnd();

	colorShader->setVec3("color", gridColor[0], gridColor[1], gridColor[2]);
	glBegin(GL_LINES);
	for (float i = spaceLength; i <= limit; i += spaceLength)
	{
		glVertexAttrib3f(idPosition, i, limit, 0);
		glVertexAttrib3f(idPosition, i, -limit, 0);
		glVertexAttrib3f(idPosition, limit, i, 0);
		glVertexAttrib3f(idPosition, -limit, i, 0);
	}
	glEnd();
}