#pragma once

#include <tuple>

#include "ELM.h"
#include "FileSystem.h"

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

	const int numActions = 4;
	const int numStates = 4; // for now the number of states is 4 since we don't include the obstruction
	int numEpisodes;
	float discountFactor, epsilon, epsDecayFactor, learningRate;

	std::vector<std::vector<MazeCell>> maze;
	std::vector<std::vector<int>> codedMaze;
	Eigen::MatrixXf qTable;

	State currentPosition = State{ -1, -1 };

	std::vector<std::pair<int, int>> bestPath;

	void EncodeMaze();
	void CreateQTable();
	void EpsilonGreedy();

	State GetStartingPosition();
	bool IsTerminalCell(State state);

	std::tuple<int, bool> TakeAction(int action);
	int GetCellValue(State state);
};