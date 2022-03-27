#pragma once

#include <vector>

#include "MazeCell.h"
#include "ObjectTracker.h"

class Agent
{
public:

	Agent();
	
	void AttachNetwork();

	void MoveUpdate();
	void RandomMove(float destX, float destY);
	
private:

	bool network = false;
	float speed = 0.35f;
};

