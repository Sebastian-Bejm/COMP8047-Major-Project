#pragma once

#include "MazeGenerator.h"
#include "ObjectTracker.h"
#include "Time.h"
#include "QLearn.h"

enum Mode {
	QLEARN,
	QELM
};

class Agent
{
public:

	Agent();
	
	void InitializeQLearn();
	void Train(Mode mode, bool verbose=false);

	void MoveUpdate();

private:

	QLearn instance;

	std::vector<MazeCell> currentPath;
	int currentPointIndex = 0;

	float agentSpeed = 0.25f;
	float errorTime = 4.0f;

	void MoveTowards(float destX, float destY);
};