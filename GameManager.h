#pragma once

#include "ObstructionGenerator.h"
#include "MazeGenerator.h"
#include "ObjectTracker.h"
#include "PhysicsWorld.h"
#include "TimeTracker.h"

class GameManager
{
public:

	static GameManager& GetInstance();
	void Attach(ObstructionGenerator* obsGenerator, MazeGenerator* mazeGenerator);

	void LoadShaders();
	void LoadScene();
	void LoadNewScene();

	void ResetScene();
	void CleanScene();

	void Update();

private:

	ObstructionGenerator* obsGenerator = nullptr;
	MazeGenerator* mazeGenerator = nullptr;

	std::vector<Shader> shaderStorage;

	bool reachedGoal = false;
	int timeAfterGoal = 0;
	const int graceTime = 1000;

	bool InTerminalState(GameObject* agent);
};