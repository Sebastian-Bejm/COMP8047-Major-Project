#pragma once

#include <vector>

#include "MazeCell.h"
#include "ObjectTracker.h"

class Agent
{
public:

	Agent();
	Agent(bool usingNetwork);
	
	void AttachNetwork();

	void MoveUpdate();
	void RandomMove(float destX, float destY);
	void NetworkMove();
	
private:

	bool usingNetwork;
	float speed = 0.35f;
};

