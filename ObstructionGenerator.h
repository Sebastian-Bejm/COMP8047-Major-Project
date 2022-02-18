#pragma once

#include <chrono>

#include "MazeGenerator.h"
#include "PhysicsWorld.h"

class ObstructionGenerator
{
public:

	ObstructionGenerator();

	void InitShaders();
	void AttachMaze(std::vector<std::vector<MazeCell>> maze);
	void SetSpawnRadius(int radius);
	void SetTimeInterval(float interval);
	void RunGenerator(ObjectTracker* tracker, PhysicsWorld* physicsWorld);
	
private:

	Shader obstacleShader;

	// the time interval will increase according to the number of obstructions in game
	// when a limit has been reached stop spawning
	float timeInterval = 0.0f;
	int generatedObstructions = 0, obstructionLimit = 6; // temp
	int spawnRadius = 0;
	bool limitReached;

	bool clockStarted = false;
	std::chrono::high_resolution_clock::time_point startTime, currentTime;

	std::vector<std::vector<MazeCell>> maze;

	GameObject& GenerateObstruction(glm::vec3 targetPosition);

	void NextFrame();
	bool IsValidLocation(GameObject* agent, GameObject* targetObject);
};

