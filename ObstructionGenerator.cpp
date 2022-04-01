#include "ObstructionGenerator.h"

ObstructionGenerator::ObstructionGenerator() {
	prevTime = clock();
	mazeGenerator = nullptr;
}

void ObstructionGenerator::AttachMazeGenerator(MazeGenerator* mazeGenerator) {
	this->mazeGenerator = mazeGenerator;
}

void ObstructionGenerator::RunGenerator(ObjectTracker* tracker, PhysicsWorld* physicsWorld) {
	// if time interval is met
	// create new object
	// check if theres a valid spot where the object can be generated
	// change its position if not valid

}

GameObject& ObstructionGenerator::GenerateObstruction(glm::vec3 targetPosition) {
	Shader obstacleShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");

	GameObject obstacle("obstruction", "crate.jpg", obstacleShader, 
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

