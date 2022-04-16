#include "ObstructionGenerator.h"

// https://stackoverflow.com/questions/7986465/algorithm-to-generate-random-position-for-game-object
// https://stackoverflow.com/questions/20364132/algorithm-to-spawn-objects-randomly
ObstructionGenerator& ObstructionGenerator::GetInstance() {
	static ObstructionGenerator instance;
	return instance;
}

void ObstructionGenerator::RunGenerator() {
	// if time interval is met
	// create new object
	// check if theres a valid spot where the object can be generated
	// change its position if not valid
}

GameObject& ObstructionGenerator::GenerateObstruction(glm::vec3 targetPosition) {
	Shader obstacleShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");

	GameObject obstacle("obstruction", "lava.png", obstacleShader, 
		targetPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));

	return obstacle;
}

void ObstructionGenerator::NextFrame() {

}

bool ObstructionGenerator::IsValidLocation(GameObject* agent, GameObject* targetObject) {
	
	RigidBody* agentRb = agent->GetRigidBody();
	float agentX = agentRb->box2dBody->GetPosition().x;
	float agentY = agentRb->box2dBody->GetPosition().y;

	float targetX = agentX + radius;


	return false;
}

