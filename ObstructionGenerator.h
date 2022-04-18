#pragma once

#include "MazeGenerator.h"
#include "ObjectTracker.h"
#include "PhysicsWorld.h"

class ObstructionGenerator
{
public:

	static ObstructionGenerator& GetInstance();

	void Update();
	void UpdateMarkedObstructions();
	
private:

	bool newMaze = true;
	bool mazeUpdated = false;

	std::vector<MazeCell> obstructions;
	std::vector<bool> generatedObstructions;

	const int revealRadius = 3; // temp, need to play around with it

	void FindObstructions();
	void GenerateObstruction(MazeCell obsCell);

	bool InRange();
};

