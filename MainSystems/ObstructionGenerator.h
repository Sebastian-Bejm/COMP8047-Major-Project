#pragma once

#include "MazeGenerator.h"
#include "ObjectTracker.h"
#include "Physics/PhysicsWorld.h"

class ObstructionGenerator
{
public:

	static ObstructionGenerator& GetInstance();

	void Update();

	void StartGenerator(bool start);
	bool GetMazeUpdates();
	
private:

	bool generatorStarted = false;
	bool mazeUpdated = false;

	std::vector<MazeCell> obstructions;

	const int revealRadius = 1.5f;

	void UpdateMarkedObstructions();
	void FindObstructions();
	void GenerateObstruction(MazeCell obsCell);

	bool InRange();
};

