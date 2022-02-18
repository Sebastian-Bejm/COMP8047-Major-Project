#include "ObstructionGenerator.h"

ObstructionGenerator::ObstructionGenerator() {

	limitReached = false;
}

void ObstructionGenerator::InitShaders() {
	obstacleShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");
}

void ObstructionGenerator::AttachMaze(std::vector<std::vector<MazeCell>> maze) {
	this->maze = maze;
}

void ObstructionGenerator::SetSpawnRadius(int radius) {
	spawnRadius = radius;
}

void ObstructionGenerator::SetTimeInterval(float interval) {
	timeInterval = interval;
}

void ObstructionGenerator::RunGenerator(ObjectTracker* tracker, PhysicsWorld* physicsWorld) {
	if (!clockStarted) {
		startTime = std::chrono::high_resolution_clock::now();
		clockStarted = true;
	}

	currentTime = std::chrono::high_resolution_clock::now();

	float duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
	if (duration > timeInterval) {
		// Spawn obsutrction here
		clockStarted = false;
	}

}

GameObject& ObstructionGenerator::GenerateObstruction(glm::vec3 targetPosition) {
	GameObject obstacle("obstruction", "tooncrate.png", obstacleShader, 
		targetPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
	generatedObstructions++;

	return obstacle;
}

void ObstructionGenerator::NextFrame() {

}

bool ObstructionGenerator::IsValidLocation(GameObject* agent, GameObject* targetObject) {
	
	RigidBody* agentRb = agent->GetRigidBody();
	float agentX = agentRb->box2dBody->GetPosition().x;
	float agentY = agentRb->box2dBody->GetPosition().y;

	float targetX = agentX + spawnRadius;


	return false;
}