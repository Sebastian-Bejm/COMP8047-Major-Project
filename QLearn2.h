#pragma once

#include "ELM.h"
#include "FileSystem.h"

// https://github.com/SohamBhure/Q-Learning-In-C/blob/master/QLearningInC.c
// https://medium.com/swlh/q-learning-using-c-language-f072fab75e1b
// https://www.samyzaf.com/ML/rl/qmaze.html
// https://www.analyticsvidhya.com/blog/2021/04/q-learning-algorithm-with-step-by-step-implementation-using-python/

class QLearn2
{
public:

	QLearn2(std::string filename, float epsilon, float learningRate, int numEpisodes) {
		this->maze = FileSystem::ReadMazeDataFile(filename);

		this->epsilon;
		this->learningRate = learningRate;
		this->numEpisodes = numEpisodes;

		qMatrix = Eigen::MatrixXf(NUM_STATES, NUM_ACTIONS);
		rMatrix = Eigen::MatrixXf(NUM_STATES, NUM_ACTIONS);
	}

	void RunTrainSession() {

	}

	void QTrain() {

	}

private:

	std::vector<std::string> ACTIONS = { "LEFT", "RIGHT", "UP", "DOWN" };
	const int NUM_ACTIONS = 4;
	const int NUM_STATES = 4;

	float epsilon, learningRate;
	int numEpisodes;

	std::vector<std::vector<MazeCell>> maze;
	Eigen::MatrixXf qMatrix, rMatrix;
};