#pragma once

#include <vector>

#include "MazeCell.h"
#include "GameObject.h"

class Agent
{
public:

	Agent();
	
	void AttachAgentObject(GameObject* agentObject);
	void AttachNetwork();

	void RandomMove();
	void NetworkMove();

	void MoveUpdate();
	void Move(GameObject* agent, float destX, float destY);
	
private:

	GameObject* agent;
	float speed = 0.35f;
	std::vector<std::vector<MazeCell>> maze;
};

