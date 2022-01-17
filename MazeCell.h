#pragma once

#include <ostream>

class MazeCell {
public:

	MazeCell();
	MazeCell(int row, int col);
	MazeCell(int row, int col, bool isWall);

	void SetWall(bool isWall);
	bool IsWall();

	int GetRow();
	int GetColumn();

	bool IsEqual(const MazeCell& mc);
	std::string str();

private:

	int row, col;
	bool isWall; // represented as '*'
};