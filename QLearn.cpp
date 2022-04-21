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
	Eigen::MatrixXd qTable = Eigen::MatrixXd::Zero(maze.size() * maze[0].size(), NUM_ACTIONS);

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

	Eigen::MatrixXd qTable = Eigen::MatrixXd::Zero(maze.size() * maze[0].size(), NUM_ACTIONS);

	// Init training samples as 2d vector
	std::vector<std::vector<double>> trainingSamples;

	auto startTime = std::chrono::system_clock::now();
	// std::vector<int> actions = { 0, 1, 2, 3 }; // left, right, up, down

	// input size is 2 - (St, At), output size is 1 - (Qt)
	ELM elm = ELM(2, 100, 1);

	QMaze qMaze(mazeNumRep, startPos, endPos);
	int currentSteps = 0;
	int episodeForELMstart = 1;
	bool ELMstart = false;

	for (int i = 0; i < numEpisodes; i++) {
		int state = qMaze.Reset();
		epsilon *= epsDecayFactor;

		bool done = false;
		int steps = 0;

		if (i >= episodeForELMstart && !ELMstart) {
			Eigen::MatrixXd samplesMatrix = CreateTrainingSampleMatrix(trainingSamples, true);
			Eigen::MatrixXd input = samplesMatrix(Eigen::all, Eigen::seq(0, 1));
			Eigen::MatrixXd output = samplesMatrix.col(2);

			Eigen::MatrixXf trainX = input.cast<float>();
			Eigen::MatrixXf trainY = output.cast<float>();

			elm.Train(trainX, trainY);

			if (!ELMstart) {

				Eigen::MatrixXf testX = trainX.bottomRows(20);
				std::cout << "Actual: " << trainY.bottomRows(20) << std::endl;

				Eigen::MatrixXf test = elm.Predict(testX);
				std::cout << "Pred: " << test << std::endl;

				ELMstart = true;
			}
		}

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

			// get results of action
			std::tuple<int, double, bool> res = qMaze.TakeAction(action);
			int newState = std::get<0>(res);
			double reward = std::get<1>(res);
			done = std::get<2>(res);

			double qValue = reward + learningRate * (discountFactor * qTable.row(newState).maxCoeff() - qTable(state, action));
			qTable(state, action) += qValue;

			// create new training sample
			std::vector<double> sample = { static_cast<double>(state), static_cast<double>(action), qTable(state, action) };
			trainingSamples.push_back(sample);

			// rolling window remove older samples
			if (trainingSamples.size() >= MAX_TRAINING_SIZE) {
				const int samplesToRemove = trainingSamples.size() - MAX_TRAINING_SIZE;
				if (samplesToRemove != 0) {
					trainingSamples.erase(trainingSamples.begin() + samplesToRemove);
				}
			}

			steps += 1;
		}


		if (verbose) {
			if (i % 100 == 0) {
				std::cout << "Run: " << i << std::endl;
				std::cout << "Steps made at this point: " << steps << std::endl;
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
		std::cout << "Training completed in: " << elapsedTimeSec << std::endl;
	}

}

// Get the path made from the last trained episode
std::vector<MazeCell> QLearn::GetPath() {
	return bestPath;
}

// Create the training sample matrix for a vector of input-output data
Eigen::MatrixXd QLearn::CreateTrainingSampleMatrix(std::vector<std::vector<double>> samples, bool normalize) {
	const int rows = samples.size();
	const int cols = 3;
	const int mazeSize = maze.size() * maze[0].size();

	Eigen::MatrixXd samplesMatrix = Eigen::MatrixXd(rows, cols);

	for (size_t i = 0; i < samples.size(); i++) {
		if (normalize) {
			samplesMatrix(i, 0) = samples[i][0] / mazeSize;
			samplesMatrix(i, 1) = samples[i][1] / NUM_ACTIONS;
		}
		else {
			samplesMatrix(i, 0) = samples[i][0];
			samplesMatrix(i, 1) = samples[i][1];
		}
		samplesMatrix(i, 2) = samples[i][2];
	}

	return samplesMatrix;
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