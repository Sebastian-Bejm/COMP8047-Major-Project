#pragma once

#include <vector>
#include <list>
#include <queue>
#include <iostream>
#include <random>

#include "MazeCell.h"

class MazeGenerator
{
public:

	MazeGenerator(int rows, int cols);

	void Generate();
	void WriteToFile();
	void PrintMaze();

	std::vector<std::vector<MazeCell>> GetMazeCells();

private:

	const int maxWallsRemoved = 50;

	int directions[4][2] = {
		{0, -2}, // north
		{0,  2}, // south
		{2,  0}, // east
		{-2, 0}, // west
	};

	int width, height;
	std::vector<std::vector<MazeCell>> mazeCells;

	std::vector<MazeCell> FrontierCellsOf(MazeCell& cell);
	std::vector<MazeCell> PassageCellsOf(MazeCell& cell);
	std::vector<MazeCell> GetCellsAround(MazeCell& cell, bool isWall);
	void ConnectCells(MazeCell& main, MazeCell& neighbour);
	bool IsValidPosition(int row, int col);
	
	std::queue<MazeCell> ConvertToQueue(std::vector<MazeCell> cells);
};

