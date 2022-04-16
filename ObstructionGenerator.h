#pragma once

#include "MazeGenerator.h"
#include "ObjectTracker.h"

class ObstructionGenerator
{
public:

	static ObstructionGenerator& GetInstance();

	void RunGenerator();
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

