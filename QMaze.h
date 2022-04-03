#pragma once

#include <vector>
#include <algorithm>
#include <Eigen/Dense>

#include "MazeCell.h"

struct Position {
	Position() {
		x = -1;
		y = -1;
	}
	Position(int x, int y) {
		this->x = x;
		this->y = y;
	}
	int x;
	int y;
};

enum ACTIONS {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class QMaze
{
public:
	// visited cells: -0.25
	// out of bounds: -0.8
	// regular move: -0.04
	// end cell: 1.0
	// wall: -0.75

	QMaze() {

	}

	QMaze(std::vector<std::vector<float>> maze, Position startPos, Position endPos) {
		this->maze = maze;
		this->startPos = startPos;
		this->endPos = endPos;

		for (size_t i = 0; i < maze.size(); i++) {
			for (size_t j = 0; j < maze[i].size(); j++) {
				if (maze[i][j] == 1.0) {
					Position cell = { 0, 0 };
					freeCells.push_back(cell);
				}
			}
		}

		RemoveCell(endPos);

	}

	void Reset(Position agentPos) {
		this->agentPos = agentPos;

	}

	void RemoveCell(Position pos) {
		freeCells.erase(
			std::remove_if(freeCells.begin(), freeCells.end(), [&](Position const& pos) {
			return pos.x == startPos.x && pos.y == startPos.y;
		}),
			freeCells.end());
	}


private:

	std::vector<std::vector<float>> maze;
	std::vector<Position> freeCells;
	Position startPos, agentPos, endPos;
};

