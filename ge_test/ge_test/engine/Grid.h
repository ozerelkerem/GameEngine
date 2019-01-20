#pragma once

class Grid
{
private:
	int lineCount;
	float spaceLength;
	unsigned int programID;
public:
	Grid(int lineCount, float spaceLength, unsigned int);
	void Draw();

};

