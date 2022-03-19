#include "Agent.h"

Agent::Agent() {}

void Agent::Receive() {

}

void Agent::Actuate() {

}

void Agent::Move(GameObject* agent, float destX, float destY) {
	float velX = 0.0f, velY = 0.0f;

	RigidBody* agentRb = agent->GetRigidBody();
	float xPos = agentRb->box2dBody->GetPosition().x;
	float yPos = agentRb->box2dBody->GetPosition().y;

	if (destX > xPos) {
		velX = speed;
	}
	else if (destX < xPos) {
		velX = -speed;
	}

	if (destY > yPos) {
		velY = speed;
	}
	else if (destY < yPos) {
		velY = -speed;
	}
	
	// Testing (loop while game is going?)
	// The agent will be able to move every step, so a loop may not be neccesary
	//for (int i = 0; i < 2; i++) {
		//agentRb->box2dBody->SetLinearVelocity(b2Vec2(velX, velY));
	//}
}

void Agent::UpdateMaze(std::vector<std::vector<MazeCell>>& maze) {
	this->maze = maze;
	// if maze is updated with an obstruction in front of agent, switch direction
	// the agent SHOULD know that there is now an object in the new spot and try to avoid that

}
