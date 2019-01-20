#pragma once
#include "Grid.h"

#include "../includes/GL/glew.h"

Grid::Grid(int lineCount, float spaceLength, unsigned int programID)
{
	this->spaceLength = spaceLength;
	this->lineCount = lineCount;

	this->programID = programID;
}

void Grid::Draw()
{
	int idPosition = glGetAttribLocation(this->programID, "position");
	int idColor = glGetAttribLocation(this->programID, "color");

	glBegin(GL_LINES);

	int limit = (spaceLength * lineCount / 2);
	for (float i = -limit; i <= limit; i += spaceLength)
	{
		glVertexAttrib3f(idColor, 0.2, 0.2, 0.2);
		glVertexAttrib3f(idColor, 0.2, 0.2, 0.2);
		glVertexAttrib3f(idPosition, i, limit, 0);
		glVertexAttrib3f(idPosition, i, -limit, 0);
		glVertexAttrib3f(idColor, 0.2, 0.2, 0.2);
		glVertexAttrib3f(idColor, 0.2, 0.2, 0.2);
		glVertexAttrib3f(idPosition, limit, i, 0);
		glVertexAttrib3f(idPosition, -limit, i, 0);
	}
	//y axis
	glVertexAttrib3f(idColor, 0, 1, 0);
	glVertexAttrib3f(idColor, 0, 1, 0);
	glVertexAttrib3f(idPosition, 0, limit, 0);
	glVertexAttrib3f(idPosition, 0, -limit, 0);
	//x axis
	glVertexAttrib3f(idColor, 1, 0, 0);
	glVertexAttrib3f(idColor, 1, 0, 0);
	glVertexAttrib3f(idPosition, limit, 0, 0);
	glVertexAttrib3f(idPosition, -limit, 0, 0);

	glEnd();
}