#pragma once

#include "ObstructionGenerator.h"
#include "MazeGenerator.h"
#include "ObjectTracker.h"
#include "Physics/PhysicsWorld.h"
#include "TimeTracker.h"
#include "AI/Agent.h"

class GameManager
{
public:

	static GameManager& GetInstance();

	void LoadShaders();
	void LoadScene();
	void Update();
	void CleanScene(bool programExit=false);

	int GetMazesCompleted();

private:

	Agent pathfindingAgent = Agent();

	std::vector<Shader> shaderStorage;

	bool reachedGoal = false;
	int timeAfterGoal = 0;
	const int graceTime = 1000;

	int mazesCompleted = 0;

	void LoadNewScene();
	void ResetScene();

	void StartAgent(GameObject* agent, bool newUpdates = false);
	bool InTerminalState(GameObject* agent);
};