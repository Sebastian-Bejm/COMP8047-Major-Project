#pragma once

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

	QLearn2(std::string filename, float epsilon, float learningRate, int numEpisodes) {
		this->maze = FileSystem::ReadMazeDataFile(filename);

		Position startPos = {};
		Position endPos = {};

		std::vector<std::vector<float>> mazeNumRep(maze.size(), std::vector<float>(maze[0].size()));

		for (size_t i = 0; i < maze.size(); i++) {
			for (size_t j = 0; j < maze[i].size(); j++) {
				if (maze[i][j].IsStart()) {
					startPos.x = j;
					startPos.y = i;
				}
				else if (maze[i][j].IsExit()) {
					endPos.x = j;
					endPos.y = i;
				}
				if (maze[i][j].IsWall()) {
					mazeNumRep[i][j] = 0.0;
				}
				else {
					mazeNumRep[i][j] = 1.0;
				}
			}
		}

		qMaze = QMaze(mazeNumRep, startPos, endPos);

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

	const int NUM_ACTIONS = 4;
	const int NUM_STATES = 4;

	float epsilon, learningRate;
	int numEpisodes;

	std::vector<std::vector<MazeCell>> maze;
	QMaze qMaze;

	Eigen::MatrixXf qMatrix, rMatrix;
};