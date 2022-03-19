#pragma once

#include <vector>

#include "MazeCell.h"
#include "GameObject.h"

class Agent
{
public:

	Agent();

	void Receive();
	void Actuate();

	void Move(GameObject* agent, float destX, float destY);
	void UpdateMaze(std::vector<std::vector<MazeCell>>& maze);

private:

	//GameObject* agentObject;
	float speed = 0.35f;
	std::vector<std::vector<MazeCell>> maze;
};

