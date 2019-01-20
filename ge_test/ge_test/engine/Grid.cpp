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
	int pid = glGetAttribLocation(this->programID, "position");
	glBegin(GL_LINES);
	for (float i = -(this->lineCount * spaceLength / 2); i <= (this->lineCount * spaceLength / 2); i += spaceLength)
	{
		glVertexAttrib3f(pid, i, 1, 0);
		glVertexAttrib3f(pid, i, -1, 0);
		glVertexAttrib3f(pid, 1, i, 0);
		glVertexAttrib3f(pid, -1, i, 0);
		
	}

	glEnd();
}