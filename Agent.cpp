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

}

void Agent::NextFrame() {
	current = std::chrono::system_clock::now();
}

void Agent::ClampPosition(GameObject* agent, MazeCell nextPos) {
	agent->SetPosition(nextPos.GetColumn(), -nextPos.GetRow());
}

bool Agent::AtNextPosition(float curX, float curY, MazeCell nextPos, float epsilon) {
	return (fabs(curX - nextPos.GetColumn()) < epsilon && fabs(curY - (-nextPos.GetRow()) < epsilon));
}


void Agent::MoveUpdate() {
	//NextFrame();

	if (pathDone) {
		std::cout << "Stack empty, creating path" << std::endl;
		std::vector<MazeCell> bestPath = instance.GetPath();
		std::cout << bestPath.size() << std::endl;
		std::reverse(bestPath.begin(), bestPath.end()); // reverse vector since stack goes in the opposite direction

		pathStack = std::stack<MazeCell>(std::deque<MazeCell>(bestPath.begin(), bestPath.end()));
		pathDone = false;
	}
	else {
		GameObject* agent = &ObjectTracker::GetInstance().GetObjectByTag("agent");

		if (!pathStack.empty()) {
			MazeCell nextMove = pathStack.top();

			if (agent != nullptr) {
				if (AtNextPosition(agent->GetRigidBody()->box2dBody->GetPosition().x, agent->GetRigidBody()->box2dBody->GetPosition().y, nextMove, 0.005f)) {
					//std::cout << "This should be the first move" << std::endl;
					ClampPosition(agent, nextMove);
					pathStack.pop();
				}
				else {
					//std::cout << mc.GetColumn() << "," << -mc.GetRow() << std::endl;
					Move(nextMove.GetColumn(), -nextMove.GetRow());
				}
			}

		}
	}
}

void Agent::Move(float destX, float destY) {
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

		// flipped test
		if (destY > yPos) {
			velY = agentSpeed;
		}
		else if (destY < yPos) {
			velY = -agentSpeed;
		}

		// The agent should be able to move every "step", so a loop may not be neccesary
		for (int i = 0; i < 2; i++) {
			agent->SetVelocity(velX, velY);
		}
	}
	
}
