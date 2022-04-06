#pragma once

#include <chrono>

#include "ELM.h"
#include "QMaze.h"
#include "FileSystem.h"

// https://github.com/SohamBhure/Q-Learning-In-C/blob/master/QLearningInC.c
// https://medium.com/swlh/q-learning-using-c-language-f072fab75e1b
// https://www.samyzaf.com/ML/rl/qmaze.html
// https://www.analyticsvidhya.com/blog/2021/04/q-learning-algorithm-with-step-by-step-implementation-using-python/

class QLearn2
{
public:

	QLearn2(std::string filename, float discountFactor, float epsilon, float epsDecayFactor, float learningRate, int numEpisodes) {
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

		qTable = Eigen::MatrixXf(numRows*numCols, NUM_ACTIONS);
	}

	void RunTrainSession() {
		std::random_device rand_dev;
		std::mt19937 gen(rand_dev());
		std::uniform_real_distribution<float> eps_distr(0, 1);
		std::uniform_int_distribution<int> actions(0, NUM_ACTIONS-1);

		auto startTime = std::chrono::system_clock::now();

		QMaze qMaze(mazeNumRep, startPos, endPos);

		for (int i = 0; i < numEpisodes; i++) {
			int state = qMaze.Reset();
			//epsilon *= epsDecayFactor;
			bool done = false;

			while (!done) {

				int action = -1;
				if (eps_distr(gen) < epsilon) {
					action = actions(gen);
				}
				else {
					int maxIndex;
					action = qTable.row(state).maxCoeff(&maxIndex);
				}

				std::tuple<int, double, bool> res = qMaze.TakeAction(action);
				int newState = std::get<0>(res);
				double reward = std::get<1>(res);
				bool isDone = std::get<2>(res);

				done = true;
			}

		}

		auto endTime = std::chrono::system_clock::now();

		double elapsedTimeMS = std::chrono::duration<double, std::milli>(endTime - startTime).count();
		double elapsedTimeSec = elapsedTimeMS / 1000;

		std::cout << "Training complete in: " << elapsedTimeSec << std::endl;
	}

	void QTrain() {

	}

private:

	const int NUM_ACTIONS = 4;
	int numRows, numCols;

	float discountFactor, epsilon, epsDecayFactor, learningRate;
	int numEpisodes;

	std::vector<std::vector<MazeCell>> maze;
	std::vector<std::vector<double>> mazeNumRep;
	State startPos, endPos;

	Eigen::MatrixXf qTable;
};