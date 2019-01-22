#pragma once

class Grid
{
public:
	int lineCount;
	float spaceLength;
	unsigned int programID;

	float gridColor[3] = { 0.2f, 0.2f, 0.2f };

	Grid(int lineCount, float spaceLength, unsigned int programID);
	void Draw();
};

