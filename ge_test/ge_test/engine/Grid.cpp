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
	
	for (float i = -1; i <= 1; i += spaceLength)
	{
		glVertexAttrib3f(pid, i, 1, 0);
		glVertexAttrib3f(pid, i, -1, 0);
		glVertexAttrib3f(pid, 1, i, 0);
		glVertexAttrib3f(pid, -1, i, 0);
		break;
	}

	glDrawArrays(GL_LINES, 0, 4);
}