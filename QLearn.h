#pragma once

#include <tuple>

#include "ELM.h"
#include "FileSystem.h"

// https://github.com/stschoberg/mazeGame
// https://github.com/jinfagang/Q-Learning/blob/master/main.cpp

struct State {
	int x;
	int y;
};

class QLearn
{
public:

	QLearn();
	QLearn(std::string filename, float discountFactor, float epsilon, float epsDecayFactor, float learningRate, int numEpisodes);

	Eigen::MatrixXf Learn();

private:

	const int NUM_STATES = 4; // for now the number of states is 4 since we don't include the obstruction
	const int NUM_ACTIONS = 4;
	std::vector<std::string> ACTIONS = { "left", "right", "up", "down" };

	int numEpisodes;
	float discountFactor, epsilon, epsDecayFactor, learningRate;

	std::vector<std::vector<MazeCell>> maze;
	std::vector<std::vector<int>> mazeValues;
	Eigen::MatrixXf qTable;

	State currentPosition = State{ -1, -1 };

	std::vector<std::pair<int, int>> bestPath;

	void CreateQTable();
	void InitMazeRewardValues();
	std::string EpsilonGreedy(State state);

	void EvaluateQ(State position, std::string action);

	bool IsTerminalCell(State state);
	bool IsValidCell(State coordinate, std::string action);

	int GetCellValue(State state);

	State GetStartingPosition();
	State GetCellAfterAction(State coordinate, std::string action);
};