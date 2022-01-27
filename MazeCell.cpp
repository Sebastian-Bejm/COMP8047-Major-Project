#include "MazeCell.h"

MazeCell::MazeCell() {
	row = -1;
	col = -1;
	isWall = false;
	obstructed = false;
	start = false;
	exit = false;
}

MazeCell::MazeCell(int row, int col) {
	this->row = row;
	this->col = col;
	isWall = false;
	obstructed = false;
	start = false;
	exit = false;
}

MazeCell::MazeCell(int row, int col, bool isWall) {
	this->row = row;
	this->col = col;
	this->isWall = isWall;
	obstructed = false;
	start = false;
	exit = false;
}

void MazeCell::SetWall(bool isWall) {
	bool old = this->isWall;
	this->isWall = isWall;
}

bool MazeCell::IsWall() {
	return isWall;
}

void MazeCell::SetObstruction(bool obstructed) {
	this->obstructed = obstructed;
}

bool MazeCell::IsObstruction() {
	return obstructed;
}

void MazeCell::SetAsStart(bool start) {
	this->start = start;
}

void MazeCell::SetAsExit(bool exit) {
	if (!this->start) {
		this->exit = exit;
	}
}

bool MazeCell::IsStart() {
	return start;
}

bool MazeCell::IsExit() {
	return exit;
}

int MazeCell::GetRow() {
	return row;
}

int MazeCell::GetColumn() {
	return col;
}

bool MazeCell::IsEqual(const MazeCell& mc) {
	return row == mc.row && col == mc.col;
}

std::string MazeCell::str() {
	if (isWall) {
		return "# ";
	}
	if (obstructed) {
		return "X ";
	}
	if (start) {
		return "S ";
	}
	if (exit) {
		return "E ";
	}
	return ". ";
}