#pragma once

#include "MazeGenerator.h"
#include "ObjectTracker.h"

class ObstructionGenerator
{
public:

	ObstructionGenerator();

	void AttachMaze(std::vector<std::vector<MazeCell>> maze);
	void RunGenerator();
	void GenerateObstruction(ObjectTracker* tracker);
	
private:

	bool IsValidLocation();

	std::vector<std::vector<MazeCell>> maze;

	const float interval = 7.5f;
	clock_t prevTime;
	clock_t deltaTime = 0;
};

