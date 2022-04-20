#pragma once

#include "MazeGenerator.h"
#include "ObjectTracker.h"
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
	void InitializeEnvironment();
	void Train(Mode mode, bool verbose=false);

	void UpdateCurrentState(glm::vec3 pos);
	void MoveUpdate();

private:

	QLearn instance;

	std::vector<MazeCell> currentPath;
	int currentPointIndex = 0;

	float agentSpeed = 0.3f;

	void MoveTowards(float destX, float destY);
};