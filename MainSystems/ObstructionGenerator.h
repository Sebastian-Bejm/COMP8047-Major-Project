#pragma once

#include "MazeGenerator.h"
#include "ObjectTracker.h"
#include "Physics/PhysicsWorld.h"

class ObstructionGenerator
{
public:

	static ObstructionGenerator& GetInstance();

	void Update();
	void UpdateMarkedObstructions();

	void StartGenerator(bool start);
	bool GetMazeUpdates();
	
private:

	bool generatorStarted = false;
	bool mazeUpdated = false;

	std::vector<MazeCell> obstructions;

	const int revealRadius = 1.5f;

	void FindObstructions();
	void GenerateObstruction(MazeCell obsCell);

	bool InRange();
};

