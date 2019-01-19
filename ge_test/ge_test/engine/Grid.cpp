#pragma once
#include "Grid.h"

#include "../includes/GL/glew.h"


Grid::Grid(int lineCount, float spaceLength)
{
	this->spaceLength = spaceLength;
	this->lineCount = lineCount;
}

void Grid::Draw()
{
	glBegin(GL_LINES);

	for (float i = -1; i <= 1; i += spaceLength)
	{
		glVertex3f(i, 1, 0);
		glVertex3f(i, -1, 0);
		glVertex3f(1, i, 0);
		glVertex3f(-1, i, 0);
	}

	glEnd();
}
