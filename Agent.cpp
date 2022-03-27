#include "Agent.h"

Agent::Agent() {}

void Agent::AttachAgentObject(GameObject* agentObject) {
	agent = agentObject;
}

void Agent::MoveUpdate() {

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
