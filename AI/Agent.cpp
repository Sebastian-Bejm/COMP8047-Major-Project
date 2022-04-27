#include "Agent.h"

Agent::Agent() {}

// Initialize the QLearn class with the hyperparameters
void Agent::InitializeHyperparameters() {
	double discountFactor = 0.85;
	double eps = 0.5;
	double epsDecayFactor = 0.998;
	double learningRate = 0.8;
	int numEpisodes = 500;

	// Initalize the QLearn class with hyperparameters
	instance.InitHyperparameters(discountFactor, eps, epsDecayFactor, learningRate, numEpisodes);
}

// Initialize the environment for learning
void Agent::InitializeEnvironment() {
	instance.AttachMazeFromGame(MazeGenerator::GetInstance().GetMazeCells());
}

// Train the QLearn internally, then pass path information to Agent
void Agent::Train(Mode mode, bool verbose) {
	// Set the training mode
	if (mode == Mode::QLEARN) {
		instance.TrainQLearn(verbose, 50);
	}
	else if (mode == Mode::QELM) {
		instance.TrainQELM(verbose, 10);
	}

	// Get the path from QLearn
	currentPath = instance.GetPath();
	currentPointIndex = 0;
}

// Update the current state of the maze for QLearn after the maze changes
void Agent::UpdateCurrentState(glm::vec3 pos) {
	instance.UpdateCurrentState(pos.x, pos.y, MazeGenerator::GetInstance().GetMazeCells());
}

// Start performing the navigation given the path from QLearn
void Agent::MoveUpdate() {
	GameObject* agent = &ObjectTracker::GetInstance().GetObjectByTag("agent");
	
	if (!currentPath.empty()) {
		MazeCell mc = currentPath[currentPointIndex];
		glm::vec3 destPoint = glm::vec3(mc.GetColumn(), -mc.GetRow(), 0);

		if (glm::distance(agent->GetTransform()->GetPosition(), destPoint) < 0.01f) {
			currentPointIndex++;
		}

		if (currentPointIndex >= currentPath.size()) {
			// Make sure the agent doesn't start moving on next run
			currentPath.clear();
			currentPointIndex = 0;
			agent->SetVelocity(0.0f, 0.0f);
		}
		else {
			MoveTowards(destPoint.x, destPoint.y);
		}

	}

}

// Move the agent towards a given destination point
void Agent::MoveTowards(float destX, float destY) {
	GameObject* agent = &ObjectTracker::GetInstance().GetObjectByTag("agent");

	if (agent != nullptr) {
		RigidBody* agentRb = agent->GetRigidBody();
		float xPos = agentRb->box2dBody->GetPosition().x;
		float yPos = agentRb->box2dBody->GetPosition().y;

		float velX = 0.0f, velY = 0.0f;
		float agentSpeed = 0.3f;

		if (destX > xPos) {
			velX = agentSpeed;
		}
		else if (destX < xPos) {
			velX = -agentSpeed;
		}

		if (destY > yPos) {
			velY = agentSpeed;
		}
		else if (destY < yPos) {
			velY = -agentSpeed;
		}

		// Set agent velocity for this step
		agent->SetVelocity(velX, velY);

	}
	
}
