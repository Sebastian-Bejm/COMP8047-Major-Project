#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>

#include "MazeCell.h"

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

	QMaze() {
		this->numRows = 0;
		this->numCols = 0;
	}

	QMaze(std::vector<std::vector<double>> maze, State agentPos, State endPos) {
		this->maze = maze;
		this->endPos = endPos;
		this->startPos = agentPos;

		this->numRows = maze.size();
		this->numCols = maze[0].size();
		//PrintMaze();
	}

	int Reset() {
		this->agentPos = startPos;
		//this->minReward = -0.5 * numRows * numCols;
		//this->totalReward = 0;
		return GetState();
	}

	std::tuple<int, double, bool> TakeAction(int action) {
		std::tuple<double, bool> result = UpdateState(action);
		float reward = std::get<0>(result);
		bool status = std::get<1>(result);
		int newState = GetState();
		//std::cout << newState << std::endl;
		//std::cout << status << std::endl;

		return std::make_tuple(newState, reward, status);
	}

	std::tuple<double, bool> UpdateState(int action) {
		//State currentState = agentPos;
		std::vector<int> validActions = GetValidActions(agentPos);

		// if action is in valid actions, then do the below:
		// if action is NOT in valid actions then its an invalid move, punish with -0.75

		// check if action is in valid actions
		if (std::find(validActions.begin(), validActions.end(), action) != validActions.end()) {

			if (action == 0) { // left
				if (agentPos.x == 0) {
					return std::make_tuple(BOUNDS_PENALTY, false);
				}
				agentPos.x -= 1;
			}
			else if (action == 1) { // right
				if (agentPos.x == numCols - 1) {
					return std::make_tuple(BOUNDS_PENALTY, false);
				}
				agentPos.x += 1;
			}
			else if (action == 2) { // up
				if (agentPos.y == 0) {
					return std::make_tuple(BOUNDS_PENALTY, false);
				}
				agentPos.y -= 1;
			}
			else if (action == 3) { // down
				if (agentPos.y == numRows - 1) {
					return std::make_tuple(BOUNDS_PENALTY, false);
				}
				agentPos.y += 1;
			}
		}
		else {
			return std::make_tuple(WALL_PENALTY, false);
		}

		// we reach the terminal point
		if (agentPos.x == endPos.x && agentPos.y == endPos.y) {
			return std::make_tuple(TARGET_REWARD, true);
		}
		// we haven't reached terminal point, still walking around
		else {
			return std::make_tuple(VALID_PENALTY, false);
		}

	}

	int GetState() {
		int row = agentPos.y;
		int col = agentPos.x;

		int finalState = -1;
		finalState = (row * numCols) + col; // current row * row size

		return finalState;
	}

	MazeCell StateToCell(int state) {

		return MazeCell(0, 0);
	}

private:

	std::vector<int> GetValidActions(State state) {
		int row = state.y;
		int col = state.x;
		std::vector<int> actions = { 0, 1, 2, 3 }; // left, right, up, down

		// up/down check
		if (row == 0) {
			actions[2] = -1;
		}
		else if (row == numRows - 1) {
			actions[3] = -1;
		}

		// left/right check
		if (col == 0) {
			actions[0] = -1;
		}
		else if (col == numCols - 1) {
			actions[1] = -1;
		}

		// blocked cell check (up/down)
		if (row > 0 && maze[row - 1][col] == 0.0) { // up
			actions[2] = -1;
		}
		if (row < numRows - 1 && maze[row + 1][col] == 0.0) { // down
			actions[3] = -1;
		}

		// blocked cell check (left/right)
		if (col > 0 && maze[row][col - 1] == 0.0) { // left
			actions[0] = -1;
		}
		if (col < numCols - 1 && maze[row][col + 1] == 0.0) { // right
			actions[1] = -1;
		}

		return actions;
	}

	void PrintMaze() {
		if (maze.size() <= 1 || maze[0].size() <= 1) {
			return;
		}
		for (size_t row = 0; row < maze.size(); row++) {
			for (size_t col = 0; col < maze[0].size(); col++) {
				std::cout << maze[row][col] << " ";
			}
			std::cout << std::endl;
		}

	}

	std::vector<std::vector<double>> maze;
	int numRows, numCols;
	//float minReward, totalReward;

	const double WALL_PENALTY = -0.75, VALID_PENALTY = -0.04, BOUNDS_PENALTY = -0.8;
	const double TARGET_REWARD = 1.0;

	State startPos, agentPos, endPos;
};