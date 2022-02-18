#pragma once

#include <iostream>

#include "GameObject.h"
#include "Agent.h"
#include "ObjectTracker.h"

class GameManager
{
public:
	GameManager();

	void Update(ObjectTracker* tracker);
	void AttachAgent(ObjectTracker* tracker);
	void ResetGame();
	
private:
	bool AgentReachedGoal();
	bool IsMazeUpdated();
	
};

