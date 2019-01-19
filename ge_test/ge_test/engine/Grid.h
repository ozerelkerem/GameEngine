#pragma once

ref class Grid
{
public:
	Grid(int lineCount, float spaceLength);

	void Draw();

private:
	int lineCount;
	float spaceLength;
};

