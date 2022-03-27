#pragma once

#include "MazeGenerator.h"
#include "ObjectTracker.h"
#include "PhysicsWorld.h"

class ObstructionGenerator
{
public:

	ObstructionGenerator();

	void AttachMazeGenerator(MazeGenerator* mazeGenerator);
	void RunGenerator(ObjectTracker* tracker, PhysicsWorld* physicsWorld);
	GameObject& GenerateObstruction(glm::vec3 targetPosition);
	
private:

	void NextFrame();
	bool IsValidLocation(GameObject* agent, GameObject* targetObject);

	MazeGenerator* mazeGenerator;

	const float interval = 5.0f;
	const int radius = 3;

	clock_t prevTime;
	clock_t deltaTime = 0;
};

