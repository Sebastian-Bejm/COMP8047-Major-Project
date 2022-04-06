#pragma once

#include <vector>

#include "ObjectTracker.h"
#include "QLearn.h"

class Agent
{
public:

	Agent();
	
	void AttachNetwork(QLearn* instance);

	void MoveUpdate();
	void Move(float destX, float destY);
	
private:

	QLearn* instance;
	float speed = 0.25f;
};

