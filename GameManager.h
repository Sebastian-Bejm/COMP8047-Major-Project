#pragma once

#include "ObstructionGenerator.h"
#include "MazeGenerator.h"
#include "ObjectTracker.h"
#include "PhysicsWorld.h"

class GameManager
{
public:

	static GameManager& GetInstance();
	void Attach(ObstructionGenerator* obsGenerator, MazeGenerator* mazeGenerator);

	void LoadShaders();
	void LoadScene();
	void ResetScene();
	void ClearScene();

	void Update();

private:

	ObstructionGenerator* obsGenerator = nullptr;
	MazeGenerator* mazeGenerator = nullptr;

	std::vector<Shader> shaderStorage;

	bool resetGame = false;

	bool InTerminalState(GameObject* agent);
};