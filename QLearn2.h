#pragma once

#include "ELM.h"

class QLearn2
{
public:
	QLearn2(std::string filename, float learningRate, int numEpisodes) {
		this->learningRate = learningRate;
		this->numEpisodes;
	}

private:

	const int NUM_ACTIONS = 4;
	const int NUM_STATES = 4;

	float learningRate;
	int numEpisodes;

};

