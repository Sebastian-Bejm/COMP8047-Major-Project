#pragma once

#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include <random>

#include "MazeCell.h"

class MazeGenerator
{
public:

	MazeGenerator();

	void InitMaze(int rows, int cols);
	void Generate();
	//void WriteToFile(); // this may be used later for the neural network
	void PrintMaze();

	std::vector<std::vector<MazeCell>> GetMazeCells();

private:

	int directions[4][2] = {
		{0, -2}, // north
		{0,  2}, // south
		{2,  0}, // east
		{-2, 0}, // west
	};

	std::vector<std::vector<MazeCell>> mazeCells;

	std::vector<MazeCell> FrontierCellsOf(MazeCell& cell);
	std::vector<MazeCell> PassageCellsOf(MazeCell& cell);
	std::vector<MazeCell> GetCellsAround(MazeCell& cell, bool isWall);

	void ConnectCells(MazeCell& main, MazeCell& neighbour);

	bool IsValidPosition(int row, int col);

	MazeCell& GetRandom(std::deque<MazeCell>& cells);
	MazeCell& GetRandom(std::vector<MazeCell>& cells);

	void PadOuterWalls();
	void CreateMazePositions();
	
	std::deque<MazeCell> ConvertToDeque(std::vector<MazeCell> cells);
};

