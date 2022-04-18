#include "ObstructionGenerator.h"

ObstructionGenerator& ObstructionGenerator::GetInstance() {
	static ObstructionGenerator instance;
	return instance;
}

void ObstructionGenerator::Update() {
	if (newMaze) {

	}
}

GameObject& ObstructionGenerator::GenerateObstruction(glm::vec3 targetPosition) {
	Shader obstacleShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");

	GameObject obstacle("obstruction", "lava.png", obstacleShader, 
		targetPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));

	return obstacle;
}

void ObstructionGenerator::GetObstructions() {
	std::vector<std::vector<MazeCell>> maze = MazeGenerator::GetInstance().GetMazeCells();
	for (size_t i = 0; i < maze.size(); i++) {
		for (size_t j = 0; j < maze[i].size(); j++) {

		}
	}
}

bool ObstructionGenerator::InRange() {
	GameObject* agent = &ObjectTracker::GetInstance().GetObjectByTag("agent");

}
