#pragma once

#include "MazeGenerator.h"
#include "ObjectTracker.h"

class ObstructionGenerator
{
public:

	static ObstructionGenerator& GetInstance();

	void Update();
	GameObject& GenerateObstruction(glm::vec3 targetPosition);
	
private:

	bool newMaze = true;
	std::vector<MazeCell> obstructions;
	const int revealRadius = 2;

	void GetObstructions();
	bool InRange();
};

