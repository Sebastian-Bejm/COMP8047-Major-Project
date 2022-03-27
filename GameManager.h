#pragma once

#include "ObstructionGenerator.h"
#include "MazeGenerator.h"
#include "ObjectTracker.h"

class GameManager
{
public:

	static GameManager& GetInstance();
	void Attach(ObstructionGenerator* obsGenerator, MazeGenerator* mazeGenerator);

	void LoadScene();
	void ResetScene();
	void ClearScene();

	void Update();

private:

	ObstructionGenerator* obsGenerator;
	MazeGenerator* mazeGenerator;

	bool resetGame = false;

	bool InTerminalState(GameObject* agent);
};