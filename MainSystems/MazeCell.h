#pragma once

#include <ostream>

class MazeCell {
public:

	MazeCell();
	MazeCell(int row, int col);
	MazeCell(int row, int col, bool isWall);

	void SetWall(bool isWall);
	bool IsWall();

	void SetObstruction(bool obstructed);
	bool IsObstruction();

	void SetAsStart(bool start);
	void SetAsExit(bool exit);
	bool IsStart();
	bool IsExit();

	void SetNewPos(int row, int col);
	int GetRow();
	int GetColumn();

	bool IsEqual(const MazeCell& mc);
	std::string str();

	friend bool operator==(const MazeCell& lhs, const MazeCell& rhs) {
		return lhs.row == rhs.row && lhs.col == rhs.col && lhs.isWall == rhs.isWall;
	}

private:

	int row, col;
	bool isWall;
	bool obstructed;
	bool start, exit;

};