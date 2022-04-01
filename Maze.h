#pragma once

#include <vector>

#include "MazeCell.h"

class Maze
{
public:
	Maze();
	Maze(int rows, int cols);

private:

	std::vector<std::vector<MazeCell>> mazeCells;
};

