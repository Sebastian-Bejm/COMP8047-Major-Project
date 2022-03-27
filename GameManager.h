#pragma once

#include "ObstructionGenerator.h"
#include "MazeGenerator.h"

class GameManager
{
public:
	GameManager();
	GameManager(ObstructionGenerator* generator, MazeGenerator* mazeGenerator);

	void LoadScene();
	void ResetScene();
	void ClearScene();

	void Update();

private:

	bool IsTerminalState();
};