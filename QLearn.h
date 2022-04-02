#pragma once

#include "ELM.h"
#include "FileSystem.h"

class QLearn
{
public:

	QLearn();
	QLearn(std::string filename, float discountFactor, float epsilon, float epsDecayFactor, float learningRate, int numEpisodes);

	void Learn();

private:
	struct State {
		int x;
		int y;
	};

	const int actions = 4;
	const int states = 4; // for now the number of states is 4 since we don't include the obstruction
	int numEpisodes;
	float discountFactor, epsilon, epsDecayFactor, learningRate;

	std::vector<std::vector<MazeCell>> maze;
	std::vector<std::vector<int>> codedMaze;
	Eigen::MatrixXf qTable;

	std::vector<std::pair<int, int>> bestPath;

	void EncodeMaze();
	void CreateQTable();
	void EpsilonGreedy();

	void UpdateMaze();
	State TakeAction(State currentState);
};