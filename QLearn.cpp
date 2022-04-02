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
}


Eigen::MatrixXf QLearn::Learn() {

	std::random_device rand_dev;
	std::mt19937 gen(rand_dev());
	std::uniform_real_distribution<float> ep_distr(0, 1);
	std::uniform_int_distribution<> action_distr(1, 4);

	for (int i = 0; i < numEpisodes; i++) {

		currentPosition = GetStartingPosition();
		epsilon *= epsDecayFactor;

		bool done = false;
		while (!done || !IsTerminalCell(currentPosition)) {
			int action = 0;

			if (ep_distr(gen) < epsilon) {
				action = action_distr(gen);
			}
			else {
				// action = max(qTable[state])
			}
			

			done = true;
		}
	}

	return qTable;
}

void QLearn::EncodeMaze() {

	codedMaze.resize(maze.size());
	for (size_t i = 0; i < codedMaze.size(); i++) {
		codedMaze[i].resize(maze[i].size());
	}	

	for (size_t r = 0; r < maze.size(); r++) {
		for (size_t c = 0; c < maze[r].size(); c++) {
			codedMaze[r][c] = maze[r][c].GetCode();
		}
	}
}

void QLearn::CreateQTable() {
	qTable = Eigen::MatrixXf::Zero(numStates, numActions);
}

std::tuple<int, bool> QLearn::TakeAction(int action) {
	State newPosition = currentPosition;

	std::random_device rand_dev;
	std::mt19937 gen(rand_dev());
	std::uniform_int_distribution<> distr(1, 4);

	bool done = false;

	int action = distr(gen);

	if (action == 1) { // move south
		if (newPosition.y == maze.size() + 1) {
			return std::make_tuple(-10, false);
		}
		else {
			currentPosition = { newPosition.x, newPosition.y + 1 };
			//return std::make_tuple(GetCellValue(currentPosition), false);
		}
	}
	else if (action == 2) { // move north
		if (newPosition.y == 0) {
			return std::make_tuple(-10, false);
		}
		else {
			currentPosition = { newPosition.x, newPosition.y - 1 };
			//return std::make_tuple(GetCellValue(currentPosition), false);
		}
	}
	else if (action == 3) { // move east
		if (newPosition.x == maze[0].size() - 1) {
			return std::make_tuple(-10, false);
		}
		else {
			currentPosition = { newPosition.x + 1, newPosition.y };
		}
	}
	else if (action == 4) { // move west
		if (newPosition.x == 0) {
			return std::make_tuple(-10, false);
		}
		else {
			currentPosition = { newPosition.x - 1, newPosition.y };
		}
	}

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

bool QLearn::IsTerminalCell(State state) {
	if (maze[state.y][state.x].str() == "E") {

	}
}

int QLearn::GetCellValue(State state) {
	// empty space
	if (maze[state.y][state.x].str() == ".") {
		return 0;
	}
	// wall
	else if (maze[state.y][state.x].str() == "#") {
		return -1;
	}
	// start
	else if (maze[state.y][state.x].str() == "S") {
		return 0;
	}
	// end / exit
	else if (maze[state.y][state.x].str() == "E") {
		return 1;
	}
	// obstruction
	else if (maze[state.y][state.x].str() == "X") {
		return -2;
	}
}
