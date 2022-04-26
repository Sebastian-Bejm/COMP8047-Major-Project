#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>

#include "MainSystems/MazeCell.h"

struct State {
	State() {
		x = -1;
		y = -1;
	}
	State(int x, int y) {
		this->x = x;
		this->y = y;
	}
	int x;
	int y;
};

class QMaze
{
public:

	QMaze(std::vector<std::vector<double>> maze, State agentPos, State endPos);

	int Reset();

	std::tuple<int, double, bool> TakeAction(int action);
	std::tuple<double, bool> UpdateState(int action);
	std::vector<MazeCell> GetPath();

	int GetState();

private:

	std::vector<std::vector<double>> maze;
	std::vector<MazeCell> path;
	int numRows, numCols;

	const double WALL_PENALTY = -0.75, VALID_PENALTY = -0.04, BOUNDS_PENALTY = -0.8;
	const double TARGET_REWARD = 1.0;

	State startPos, agentPos, endPos;

	void UpdatePath(State currentState);
	void PrintMaze();
	std::vector<int> GetValidActions(State state);
};