#include "Agent.h"

Agent::Agent() {}

// Initialize the QLearn class with the hyperparameters
void Agent::InitializeQLearn() {
	double discountFactor = 0.8;
	double eps = 0.5;
	double epsDecayFactor = 0.998;
	double learningRate = 0.1;
	int numEpisodes = 2000;

	// Initalize the QLearn class with hyperparameters
	instance.InitHyperparameters(discountFactor, eps, epsDecayFactor, learningRate, numEpisodes);
}

void Agent::AttachCurrentMaze(std::vector<std::vector<MazeCell>> currentMaze) {
	instance.AttachMazeFromGame(currentMaze);
}

// Train the QLearn internally, then the Agent will have access to path information
void Agent::Train(Mode mode, bool verbose) {
	// Get the current maze
	instance.AttachMazeFromGame(MazeGenerator::GetInstance().GetMazeCells());

	if (mode == Mode::QLEARN) {
		instance.TrainQLearn(verbose);
	}
	else if (mode == Mode::QELM) {
		instance.TrainQELM(verbose);
	}

	std::vector<MazeCell> bestPath = instance.GetPath();


	//for (MazeCell mc : bestPath) {
	//	std::cout << mc.GetColumn() << "," << -mc.GetRow() << std::endl;
	//}
}

void Agent::NextFrame() {
	current = std::chrono::system_clock::now();
}

void Agent::MoveUpdate() {
	NextFrame();
}

void Agent::Move(float destX, float destY) {
	GameObject* agent = &ObjectTracker::GetInstance().GetObjectByTag("agent");

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
	
	// The agent should be able to move every "step", so a loop may not be neccesary
	for (int i = 0; i < 2; i++) {
		agent->SetVelocity(velX, velY);
	}
}
