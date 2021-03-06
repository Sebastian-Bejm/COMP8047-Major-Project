#pragma once

#include <chrono>

#include "ELM.h"
#include "QMaze.h"
#include "Utility/FileSystem.h"

class QLearn
{
public:

	QLearn();
	void InitHyperparameters(double discountFactor, double epsilon, double epsDecayFactor, double learningRate, int numEpisodes);

	void AttachMazeFromFile(std::string filename);
	void AttachMazeFromGame(std::vector<std::vector<MazeCell>> maze);

	void UpdateCurrentState(float posX, float posY, std::vector<std::vector<MazeCell>> currentMaze);

	void TrainQLearn(bool verbose = false, int logRate = 50);
	void TrainQELM(bool verbose = false, int logRate = 50);

	std::vector<MazeCell> GetPath();

private:

	const int NUM_ACTIONS = 4;
	const int MAX_TRAINING_SIZE = 200;

	double discountFactor, epsilon, epsDecayFactor, learningRate;
	int numEpisodes;

	std::vector<std::vector<MazeCell>> maze;
	std::vector<MazeCell> bestPath;

	Eigen::MatrixXd CreateTrainingSampleMatrix(std::vector<std::vector<double>> samples, bool normalize);

	std::vector<std::vector<double>> CreateMazeNumRep(State& startPos, State& endPos);
};