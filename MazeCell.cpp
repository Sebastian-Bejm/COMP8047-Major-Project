#include "MazeCell.h"

MazeCell::MazeCell() {
	row = -1;
	col = -1;
	isWall = false;
}

MazeCell::MazeCell(int row, int col) {
	this->row = row;
	this->col = col;
	isWall = false;
}

MazeCell::MazeCell(int row, int col, bool isWall) {
	this->row = row;
	this->col = col;
	this->isWall = isWall;
}

void MazeCell::SetWall(bool isWall) {
	bool old = this->isWall;
	this->isWall = isWall;
}

bool MazeCell::IsWall() {
	return isWall;
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
	return "- ";
}