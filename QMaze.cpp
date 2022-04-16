#include "QMaze.h"

QMaze::QMaze(std::vector<std::vector<double>> maze, State agentPos, State endPos) {
	this->maze = maze;
	this->endPos = endPos;
	this->startPos = agentPos;

	this->numRows = maze.size();
	this->numCols = maze[0].size();
}

// Reset the simulation: reset the agent back to original position and reset the current path
int QMaze::Reset() {
	this->agentPos = startPos;
	currentPath.clear();

	MazeCell startCell = MazeCell(agentPos.y, agentPos.x);
	currentPath.push_back(startCell);

	return GetState();
}

// Take an action and return the new state, the reward, and game status
std::tuple<int, double, bool> QMaze::TakeAction(int action) {
	std::tuple<double, bool> result = UpdateState(action);
	double reward = std::get<0>(result);
	bool status = std::get<1>(result);
	int newState = GetState();

	return std::make_tuple(newState, reward, status);
}

// Update to a new state given an action
std::tuple<double, bool> QMaze::UpdateState(int action) {
	std::vector<int> validActions = GetValidActions(agentPos);

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
	// if action wasn't in valid actions then penalize 
	else {
		return std::make_tuple(WALL_PENALTY, false);
	}

	// we reach the terminal point
	if (agentPos.x == endPos.x && agentPos.y == endPos.y) {
		UpdatePath(agentPos);
		return std::make_tuple(TARGET_REWARD, true);
	}
	// we haven't reached terminal point, still walking around
	else {
		UpdatePath(agentPos);
		return std::make_tuple(VALID_PENALTY, false);
	}

}

// Return the path made from the current simulation
std::vector<MazeCell> QMaze::GetPath() {
	return bestPath;
}

// Get the state of the current agent position in the maze as an integer
int QMaze::GetState() {
	int row = agentPos.y;
	int col = agentPos.x;

	int finalState = -1;
	// the state as a single num
	finalState = (row * numCols) + col;

	return finalState;
}

// Update the current path when the agent takes a step
void QMaze::UpdatePath(State currentState) {
	MazeCell cell = MazeCell(currentState.y, currentState.x);
	currentPath.push_back(cell);
}

// Return the valid actions given the current state of the agent
std::vector<int> QMaze::GetValidActions(State state) {
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

// Prints this maze. Primarily used for debugging.
void QMaze::PrintMaze() {
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