#pragma once

#include <stack>
#include <algorithm>

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
	void Train(Mode mode, bool verbose=false);

	void MoveUpdate();
	void Move(float destX, float destY);

private:

	QLearn instance;

	std::stack<MazeCell> pathStack;

	float agentSpeed = 0.2f;
	bool pathDone = true;
	std::chrono::system_clock::time_point start, current, end;

	void NextFrame();
	bool AtNextPosition(float curX, float curY, MazeCell nextPos, float epsilon);
	void ClampPosition(GameObject* agent, MazeCell nextPos);
};

