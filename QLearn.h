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

	QLearn();
	QLearn(std::string filename, float discountFactor, float epsilon, float epsDecayFactor, float learningRate, int numEpisodes);

	void RunTrainSession(bool verbose = false);

	std::vector<MazeCell> GetPath();

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