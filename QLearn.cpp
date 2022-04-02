#include "QLearn.h"

QLearn::QLearn() {
	this->discountFactor = 0.0;
	this->epsilon = 0.0;
	this->epsDecayFactor = 0.0;
	this->learningRate = 0.0;
	this->numEpisodes = 1;
}

QLearn::QLearn(std::string filename, float discountFactor, float epsilon, float epsDecayFactor, float learningRate, int numEpisodes) {
	this->maze = FileSystem::ReadMazeDataFile(filename);

	this->discountFactor = discountFactor;
	this->epsilon = epsilon;
	this->epsDecayFactor = epsDecayFactor;
	this->learningRate = learningRate;
	this->numEpisodes = numEpisodes;

	CreateQTable();
	InitMazeRewardValues();
}

// Perform the QLearn algorithm (no network)
Eigen::MatrixXf QLearn::Learn() {
	for (int i = 0; i < numEpisodes; i++) {
		currentPosition = GetStartingPosition();

		bool done = false;
		while (!done) {			
			std::string action = EpsilonGreedy(currentPosition);
			//std::cout << action << std::endl;

			currentPosition = GetCellAfterAction(currentPosition, action);
			//std::cout << currentPosition.x << ", " << currentPosition.y << std::endl;

			if (IsTerminalCell(currentPosition)) {
				break;
			}
			done = true;
		}
	}

	return qTable;
}

void QLearn::CreateQTable() {
	qTable = Eigen::MatrixXf::Zero(NUM_STATES, NUM_ACTIONS);

	/*for (size_t r = 0; r < maze.size(); r++) {
		for (size_t c = 0; c < maze[r].size(); c++) {
			State cell = State{ (int)c, (int)r };

			for (std::string action : ACTIONS) {
				if (IsValidCell(cell, action)) {

				}
			}
		}
	}*/
}

void QLearn::InitMazeRewardValues() {
	mazeValues.resize(maze.size());
	for (size_t i = 0; i < maze.size(); i++) {
		mazeValues[i].resize(maze[i].size());
	}

	for (size_t r = 0; r < mazeValues.size(); r++) {
		for (size_t c = 0; c < mazeValues[r].size(); c++) {
			if (maze[r][c].str() == "#") {
				mazeValues[r][c] = -10;
			}
			else if (maze[r][c].str() == ".") {
				mazeValues[r][c] = -1;
			}
			else if (maze[r][c].str() == "S") {
				mazeValues[r][c] = 0;
			}
			else if (maze[r][c].str() == "E") {
				mazeValues[r][c] = 100;
			}
		}
	}
}


std::string QLearn::EpsilonGreedy(State state) {
	std::random_device rand_dev;
	std::mt19937 gen(rand_dev());
	std::uniform_real_distribution<float> ep_distr(0, 1);
	std::uniform_int_distribution<> action_distr(1, NUM_ACTIONS);

	std::string action = "";

	if (ep_distr(gen) < epsilon) {
		action = action_distr(gen);

		std::vector<State> validActions;

		for (int i = 0; i < NUM_ACTIONS; i++) {

		}

	}
	else {
		
	}

	return action;
}

State QLearn::GetStartingPosition() {
	for (size_t r = 0; r < maze.size(); r++) {
		for (size_t c = 0; c < maze[r].size(); c++) {
			if (maze[r][c].str() == "S") {
				return State{ (int)c, (int)r };
			}
		}
	}
}

// Check if the current state is in a terminal state
bool QLearn::IsTerminalCell(State state) {
	if (maze[state.y][state.x].str() == "E") {
		return true;
	}
	return false;
}

bool QLearn::IsValidCell(State coordinate, std::string action) {
	State cell = GetCellAfterAction(coordinate, action);
	return 0 <= cell.x < maze.size() && 0 <= cell.y < maze[0].size();
}

State QLearn::GetCellAfterAction(State coordinate, std::string action) {
	State newState = coordinate;
	if (action == "up") { // north
		newState.y -= 1;
	}
	else if (action == "down") { // south
		newState.y += 1;
	}
	else if (action == "right") { // east
		newState.x += 1;
	}
	else if (action == "left") { // west
		newState.x -= 1;
	}

	return newState;
}

int QLearn::GetCellValue(State state) {
	return mazeValues[state.y][state.x];
}
