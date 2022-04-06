#pragma once

#include <chrono>

#include "ELM.h"
#include "QMaze.h"
#include "FileSystem.h"

// https://www.samyzaf.com/ML/rl/qmaze.html
// https://www.analyticsvidhya.com/blog/2021/04/q-learning-algorithm-with-step-by-step-implementation-using-python/

class QLearn
{
public:

	QLearn(std::string filename, float discountFactor, float epsilon, float epsDecayFactor, float learningRate, int numEpisodes) {
		this->maze = FileSystem::ReadMazeDataFile(filename);

		this->numRows = maze.size();
		this->numCols = maze[0].size();

		this->discountFactor = discountFactor;
		this->epsilon = epsilon;
		this->epsDecayFactor = epsDecayFactor;
		this->learningRate = learningRate;
		this->numEpisodes = numEpisodes;

		State startPos = {};
		State endPos = {};
		std::vector<std::vector<double>> mazeNumRep(maze.size(), std::vector<double>(maze[0].size()));

		for (size_t i = 0; i < maze.size(); i++) {
			for (size_t j = 0; j < maze[i].size(); j++) {
				if (maze[i][j].IsStart()) {
					startPos.x = (int)j;
					startPos.y = (int)i;
				}
				else if (maze[i][j].IsExit()) {
					endPos.x = (int)j;
					endPos.y = (int)i;
				}
				if (maze[i][j].IsWall()) {
					mazeNumRep[i][j] = 0.0;
				}
				else {
					mazeNumRep[i][j] = 1.0;
				}
			}
		}

		this->mazeNumRep = mazeNumRep;
		this->startPos = startPos;
		this->endPos = endPos;

		qTable = Eigen::MatrixXf::Zero((numRows * numCols), NUM_ACTIONS);
	}

	void RunTrainSession(bool verbose=false) {
		std::random_device rand_dev;
		std::mt19937 gen(rand_dev());
		std::uniform_real_distribution<float> eps_distr(0, 1);
		std::uniform_int_distribution<int> actions(0, NUM_ACTIONS-1);

		auto startTime = std::chrono::system_clock::now();

		QMaze qMaze(mazeNumRep, startPos, endPos);
		int finalSteps = 0;

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

				qTable(state, action) = (1-learningRate)*qTable(state, action) + learningRate * 
					(reward + discountFactor * qTable.row(newState).maxCoeff() - qTable(state, action));

				steps += 1;
			}

			if (verbose) {
				if (i % 100 == 0) {
					std::cout << "Run: " << i << std::endl;
					std::cout << steps << std::endl;
				}
				if (i == numEpisodes - 1) {
					std::cout << "Steps on final run: " << steps << std::endl;
					finalSteps = steps;
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

	std::vector<MazeCell> GetPath() {
		return bestPath;
	}

private:

	const int NUM_ACTIONS = 4;
	int numRows, numCols;

	double discountFactor, epsilon, epsDecayFactor, learningRate;
	int numEpisodes;

	std::vector<std::vector<MazeCell>> maze;
	std::vector<std::vector<double>> mazeNumRep;
	std::vector<MazeCell> bestPath;
	State startPos, endPos;

	Eigen::MatrixXf qTable;
};