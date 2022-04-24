#pragma once

#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <random>

#include "MazeCell.h"
#include "FileSystem.h"

class MazeGenerator
{
public:

	static MazeGenerator& GetInstance();

	void InitMaze(int rows, int cols, int wallsToRemove = 0);
	void InitMaze(std::string filename);
	void Generate();
	void PrintMaze();

	MazeCell& GetStartCell();
	MazeCell& GetEndCell();

	std::vector<std::vector<MazeCell>>& GetMazeCells();

private:

	int wallsToRemove = 0;
	int wallRemovalLimit = 300;

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

	MazeCell& GetRandom(std::vector<MazeCell>& cells);
	
	void PadOuterWalls();
	void CreateMazePositions();

	void RemoveWalls();
	bool RemoveWall(int row, int col);
	void MarkObstructions();
};