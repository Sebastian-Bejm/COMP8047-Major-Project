#pragma once

#include <chrono>

#include "ELM.h"
#include "QMaze.h"
#include "FileSystem.h"

class QLearn
{
public:

	QLearn();
	void InitHyperparameters(double discountFactor, double epsilon, double epsDecayFactor, double learningRate, int numEpisodes);

	void AttachMazeFromFile(std::string filename);
	void AttachMazeFromGame(std::vector<std::vector<MazeCell>> maze);

	void UpdateCurrentState(float posX, float posY, std::vector<std::vector<MazeCell>> currentMaze);

	void TrainQLearn(bool verbose = false);
	void TrainQELM(bool verbose = false);

	std::vector<MazeCell> GetPath();

private:

	const int NUM_ACTIONS = 4;
	int numRows, numCols;

	double discountFactor, epsilon, epsDecayFactor, learningRate;
	int numEpisodes;

	std::vector<std::vector<MazeCell>> maze;
	std::vector<MazeCell> bestPath;

	Eigen::MatrixXf CreateTrainingSampleMatrix(std::vector<std::vector<double>> samples);

	std::vector<std::vector<double>> CreateMazeNumRep(State& startPos, State& endPos);
};