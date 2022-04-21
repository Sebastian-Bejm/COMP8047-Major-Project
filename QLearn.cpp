#include "QLearn.h"

QLearn::QLearn() {
	this->numRows = 0;
	this->numCols = 0;

	this->discountFactor = 0;
	this->epsilon = 0;
	this->epsDecayFactor = 0;
	this->learningRate = 0;
	this->numEpisodes = 0;
}

// Initialize the hyperparameter for the QLearning algorithm
void QLearn::InitHyperparameters(double discountFactor, double epsilon, double epsDecayFactor, double learningRate, int numEpisodes) {
	this->discountFactor = discountFactor;
	this->epsilon = epsilon;
	this->epsDecayFactor = epsDecayFactor;
	this->learningRate = learningRate;
	this->numEpisodes = numEpisodes;
}

// Attach a maze loaded from a file
void QLearn::AttachMazeFromFile(std::string filename) {
	this->maze = FileSystem::ReadMazeDataFile(filename);
}

// Attach the current maze directly received from the running game
void QLearn::AttachMazeFromGame(std::vector<std::vector<MazeCell>> maze) {
	this->maze = maze;
}

// Update the maze and start position with the agent's current position
void QLearn::UpdateCurrentState(float posX, float posY, std::vector<std::vector<MazeCell>> currentMaze) {
	this->maze = currentMaze;

	for (size_t i = 0; i < maze.size(); i++) {
		for (size_t j = 0; j < maze[i].size(); j++) {
			if (maze[i][j].IsStart()) {
				maze[i][j].SetAsStart(false);
			}
		}
	}

	int x = abs(std::round(posX));
	int y = abs(std::round(posY)); 

	maze[y][x].SetAsStart(true);
}

// Train using the standard Q-Learning algorithm
void QLearn::TrainQLearn(bool verbose) {
	// Create a number representation of the maze to be passed to QMaze
	// Set start and end positions
	State startPos, endPos;
	std::vector<std::vector<double>> mazeNumRep = CreateMazeNumRep(startPos, endPos);

	// Init uniform generators
	std::random_device rand_dev;
	std::mt19937 gen(rand_dev());
	std::uniform_real_distribution<float> eps_distr(0, 1);
	std::uniform_int_distribution<int> actions(0, NUM_ACTIONS - 1);

	// Init QTable
	Eigen::MatrixXf qTable = Eigen::MatrixXf::Zero(maze.size() * maze[0].size(), NUM_ACTIONS);

	auto startTime = std::chrono::system_clock::now();

	QMaze qMaze(mazeNumRep, startPos, endPos);
	int currentSteps = 0;

	for (int i = 0; i < numEpisodes; i++) {
		int state = qMaze.Reset();
		epsilon *= epsDecayFactor;

		bool done = false;
		int steps = 0;

		while (!done) {
			state = qMaze.GetState();

			int action = -1;
			if (eps_distr(gen) < epsilon) {
				action = actions(gen);
			}
			else {
				int maxIndex;
				qTable.row(state).maxCoeff(&maxIndex);
				action = maxIndex;
			}

			std::tuple<int, double, bool> res = qMaze.TakeAction(action);
			int newState = std::get<0>(res);
			double reward = std::get<1>(res);
			done = std::get<2>(res);

			qTable(state, action) += reward + learningRate * (discountFactor * qTable.row(newState).maxCoeff() - qTable(state, action));

			steps += 1;
		}

		if (verbose) {
			if (i % 100 == 0) {
				std::cout << "Run: " << i << std::endl;
				std::cout << steps << std::endl;
			}
			if (i == numEpisodes - 1) {
				std::cout << "Steps on final run: " << steps << std::endl;
				currentSteps = steps;
			}
		}

	}

	auto endTime = std::chrono::system_clock::now();

	double elapsedTimeMS = std::chrono::duration<double, std::milli>(endTime - startTime).count();
	double elapsedTimeSec = elapsedTimeMS / 1000;

	if (verbose) {
		std::cout << "Training complete in: " << elapsedTimeSec << std::endl;
	}

	this->bestPath = qMaze.GetPath();
}

// Train using the ELM based Q-Learning algorithm
void QLearn::TrainQELM(bool verbose) {
	// Create a number representation of the maze to be passed to QMaze
	// Set start and end positions
	State startPos, endPos;
	std::vector<std::vector<double>> mazeNumRep = CreateMazeNumRep(startPos, endPos);

	// Init uniform generators
	std::random_device rand_dev;
	std::mt19937 gen(rand_dev());
	std::uniform_real_distribution<float> eps_distr(0, 1);
	std::uniform_int_distribution<int> actions(0, NUM_ACTIONS - 1);

	Eigen::MatrixXf qTable = Eigen::MatrixXf::Zero(maze.size() * maze[0].size(), NUM_ACTIONS);

	Eigen::MatrixXf trainingSamples = Eigen::MatrixXf(3, 3);
	trainingSamples << 6, 6, 6, 7, 7, 7, 8, 8, 8;
	//std::cout << trainingSamples << std::endl;
	Eigen::Vector3f row = Eigen::Vector3f(1, 3);
	row << 1, 2, 3;
	trainingSamples = AddSamples(trainingSamples, row);
	std::cout << "After add:\n" << trainingSamples << std::endl;

	trainingSamples = RollWindow(trainingSamples, 2);
	std::cout << "Rolling:\n" << trainingSamples << std::endl;

	auto startTime = std::chrono::system_clock::now();
	// std::vector<int> actions = { 0, 1, 2, 3 }; // left, right, up, down

	// input size is 2 - (St, At)
	// output size is 1 - (Qt)
	ELM elm = ELM(2, 100, 1);

	QMaze qMaze(mazeNumRep, startPos, endPos);
	int currentSteps = 0;
	int episodeForELMstart = 5;

	for (int i = 0; i < 100; i++) {
		int state = qMaze.Reset();
		epsilon *= epsDecayFactor;

		bool done = false;
		int steps = 0;

		while (!done) {
			state = qMaze.GetState();

			int action = -1;
			if (eps_distr(gen) < epsilon) {
				action = actions(gen);
			}
			else {
				int maxIndex;
				qTable.row(state).maxCoeff(&maxIndex);
				action = maxIndex;
			}

			std::tuple<int, double, bool> res = qMaze.TakeAction(action);
			int newState = std::get<0>(res);
			double reward = std::get<1>(res);
			done = std::get<2>(res);

			qTable(state, action) += reward + learningRate * (discountFactor * qTable.row(newState).maxCoeff() - qTable(state, action));

			steps += 1;
		}
	}

	auto endTime = std::chrono::system_clock::now();

	double elapsedTimeMS = std::chrono::duration<double, std::milli>(endTime - startTime).count();
	double elapsedTimeSec = elapsedTimeMS / 1000;

	if (verbose) {
		std::cout << "Training completed in: " << elapsedTimeSec << std::endl;
	}

}

// Get the path made from the last trained episode
std::vector<MazeCell> QLearn::GetPath() {
	return bestPath;
}

// Roll the window for the training samples to discard the oldest samples
Eigen::MatrixXf QLearn::RollWindow(Eigen::MatrixXf samples, int windowSize) {
	int numRows = samples.rows()-1;
	int numCols = samples.cols();

	// e.g. window size is 100 and samples size is 150, we want to discard oldest 50 samples at time t

	if (samples.rows() >= windowSize) {
		Eigen::MatrixXf newSamples = samples.bottomRows(windowSize);
		return newSamples;
	}
}

// Add a new row of samples to the matrix
Eigen::MatrixXf QLearn::AddSamples(Eigen::MatrixXf samples, Eigen::Vector3f values) {
	int newRows = samples.rows() + 1;

	Eigen::MatrixXf newSamples = samples.replicate(1, 1);
	newSamples.conservativeResize(newRows, samples.cols());
	newSamples.row(static_cast<Eigen::Index>(newRows-1)) = values;

	return newSamples;
}

// Convert the maze for the QMaze class for navigation simulation
std::vector<std::vector<double>> QLearn::CreateMazeNumRep(State& startPos, State& endPos) {
	std::vector<std::vector<double>> mazeNumRep(maze.size(), std::vector<double>(maze[0].size()));

	for (size_t i = 0; i < maze.size(); i++) {
		for (size_t j = 0; j < maze[i].size(); j++) {
			if (maze[i][j].IsStart()) {
				startPos.x = maze[i][j].GetColumn();
				startPos.y = maze[i][j].GetRow();
			}
			else if (maze[i][j].IsExit()) {
				endPos.x = maze[i][j].GetColumn();
				endPos.y = maze[i][j].GetRow();
			}
			if (maze[i][j].IsWall()) {
				mazeNumRep[i][j] = 0.0;
			}
			else {
				// Initially if the cell is nothing or is an obstruction, mark it as path here
				mazeNumRep[i][j] = 1.0;
			}
		}
	}

	return mazeNumRep;
}