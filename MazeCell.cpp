#include "MazeCell.h"

// Initialize a MazeCell with default variables
MazeCell::MazeCell() {
	row = -1;
	col = -1;
	isWall = false;
	obstructed = false;
	start = false;
	exit = false;
}

// Initialize a MazeCell with a row and column
MazeCell::MazeCell(int row, int col) {
	this->row = row;
	this->col = col;
	isWall = false;
	obstructed = false;
	start = false;
	exit = false;
}

// Initialize a MazeCell with wall status
MazeCell::MazeCell(int row, int col, bool isWall) {
	this->row = row;
	this->col = col;
	this->isWall = isWall;
	obstructed = false;
	start = false;
	exit = false;
}

// Sets a cell with wall status
void MazeCell::SetWall(bool isWall) {
	this->isWall = isWall;
	if (isWall) {
		this->obstructed = false;
	}
}

// Get a cells wall status
bool MazeCell::IsWall() {
	// check for obstruced as well since it will eventually be a wall
	return isWall;
}

// Set a cell with obstruction status
void MazeCell::SetObstruction(bool obstructed) {
	this->obstructed = obstructed;
	if (obstructed) {
		this->isWall = false;
	}
}

// Get a cells obstruction status
bool MazeCell::IsObstruction() {
	return obstructed;
}

// Set a cell as the start
void MazeCell::SetAsStart(bool start) {
	this->start = start;
}

// Set a cell as the exit
void MazeCell::SetAsExit(bool exit) {
	if (!this->start) {
		this->exit = exit;
	}
}

// Get a cells start status
bool MazeCell::IsStart() {
	return start;
}

// Get a cells exit status
bool MazeCell::IsExit() {
	return exit;
}

// Set a cell with a new position
void MazeCell::SetNewPos(int row, int col) {
	this->row = row;
	this->col = col;
}

// Get a cells row num
int MazeCell::GetRow() {
	return row;
}

// Get a cells column num
int MazeCell::GetColumn() {
	return col;
}

bool MazeCell::IsEqual(const MazeCell& mc) {
	return row == mc.row && col == mc.col 
		&& mc.obstructed == mc.obstructed 
		&& mc.isWall == mc.isWall 
		&& mc.start == mc.start
		&& mc.exit == mc.exit;
}

// Get the cells string representation
std::string MazeCell::str() {
	if (obstructed) {
		return "X";
	}
	else if (start) {
		return "S";
	}
	else if (exit) {
		return "E";
	}
	else if (isWall) {
		return "#";
	}
	else {
		return ".";
	}
}