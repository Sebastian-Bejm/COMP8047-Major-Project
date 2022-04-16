#include "Agent.h"

Agent::Agent() {}

// Initialize the QLearn class with the hyperparameters
void Agent::InitializeQLearn() {
	double discountFactor = 0.95;
	double eps = 0.5;
	double epsDecayFactor = 0.998;
	double learningRate = 0.8;
	int numEpisodes = 1400;

	// Initalize the QLearn class with hyperparameters
	instance.InitHyperparameters(discountFactor, eps, epsDecayFactor, learningRate, numEpisodes);
}

// Train the QLearn internally, then pass path information to Agent
void Agent::Train(Mode mode, bool verbose) {
	// Get the current maze
	instance.AttachMazeFromGame(MazeGenerator::GetInstance().GetMazeCells());

	if (mode == Mode::QLEARN) {
		instance.TrainQLearn(verbose);
	}
	else if (mode == Mode::QELM) {
		instance.TrainQELM(verbose);
	}
	// Get the path from QLearn
	currentPath = instance.GetPath();
}

// https://forum.unity.com/threads/how-can-i-move-an-object-from-point-to-point-in-an-array-of-points.945488/
// Start performing the navigation given the path from QLearn
void Agent::MoveUpdate() {
	GameObject* agent = &ObjectTracker::GetInstance().GetObjectByTag("agent");
	
	MazeCell mc = currentPath[currentPointIndex];
	glm::vec3 destPoint = glm::vec3(mc.GetColumn(), -mc.GetRow(), 0);

	if (glm::distance(agent->GetTransform()->GetPosition(), destPoint) < 0.01f) {
		currentPointIndex++;
		std::cout << currentPointIndex << std::endl;
	}

	if (currentPointIndex >= currentPath.size()) {
		// something to make sure the path restarts on next run and doesnt overshoot
	}

	MoveTowards(destPoint.x, destPoint.y);
}

// Move the agent towards a given destination point
void Agent::MoveTowards(float destX, float destY) {
	GameObject* agent = &ObjectTracker::GetInstance().GetObjectByTag("agent");

	if (agent != nullptr) {
		RigidBody* agentRb = agent->GetRigidBody();
		float xPos = agentRb->box2dBody->GetPosition().x;
		float yPos = agentRb->box2dBody->GetPosition().y;

		float velX = 0.0f, velY = 0.0f;

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
