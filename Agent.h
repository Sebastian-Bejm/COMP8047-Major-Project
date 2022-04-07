#pragma once

#include <vector>

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
	void AttachCurrentMaze(std::vector<std::vector<MazeCell>> currentMaze);
	void Train(Mode mode, bool verbose);

	void MoveUpdate();
	void Move(float destX, float destY);
	void TeleportMove();

private:

	QLearn instance;
	float speed = 0.25f;

	std::chrono::system_clock::time_point start, current, end;

	void NextFrame();
};

