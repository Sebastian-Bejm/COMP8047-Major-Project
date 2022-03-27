#pragma once

#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <random>

#include "MazeCell.h"

class MazeGenerator
{
public:

	MazeGenerator();

	void InitMaze(int rows, int cols);
	void InitWalledEmptyMaze(int rows, int cols);
	void Generate();
	void PrintMaze();
	
	void WriteMazeData();
	void ReadMazeData();

	MazeCell& GetStartCell();
	MazeCell& GetEndCell();
	std::vector<int> GetStartCoordinates();
	std::vector<int> GetEndCoordinates();

	std::vector<std::vector<MazeCell>>& GetMazeCells();

private:

	int directions[4][2] = {
		{0, -2}, // north
		{0,  2}, // south
		{2,  0}, // east
		{-2, 0}, // west
	};

	std::vector<std::vector<MazeCell>> mazeCells;
	std::vector<std::vector<int>> encodedMaze;

	std::vector<MazeCell> FrontierCellsOf(MazeCell& cell);
	std::vector<MazeCell> PassageCellsOf(MazeCell& cell);
	std::vector<MazeCell> GetCellsAround(MazeCell& cell, bool isWall);

	void ConnectCells(MazeCell& main, MazeCell& neighbour);

	bool IsValidPosition(int row, int col);

	MazeCell& GetRandom(std::vector<MazeCell>& cells);
	
	void EncodeMaze();
	void PadOuterWalls();
	void CreateMazePositions();
};