#include "Agent.h"

Agent::Agent() {}

// Initialize the QLearn class with the hyperparameters
void Agent::InitializeQLearn() {
	double discountFactor = 0.95;
	double eps = 0.5;
	double epsDecayFactor = 0.998;
	double learningRate = 0.8;
	int numEpisodes = 1500;

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

void Agent::ClampPosition(GameObject* agent, MazeCell nextPos) {
	agent->SetPosition(nextPos.GetColumn(), -nextPos.GetRow());
}

bool Agent::AtNextPosition(float curX, float curY, MazeCell nextPos, float epsilon) {
	return (fabs(curX - nextPos.GetColumn()) < epsilon && fabs(curY - (-nextPos.GetRow()) < epsilon));
}

void Agent::MoveUpdate() {
	if (pathDone) {
		std::vector<MazeCell> bestPath = instance.GetPath();
		for (MazeCell mc : bestPath) {
			std::cout << mc.GetColumn() << "," << -mc.GetRow() << std::endl;
		}

		std::reverse(bestPath.begin(), bestPath.end()); // reverse vector since stack goes in the opposite direction
		//bestPath.erase(bestPath.begin());
		pathStack = std::stack<MazeCell>(std::deque<MazeCell>(bestPath.begin(), bestPath.end()));
		pathDone = false;
	}
	else {
		GameObject* agent = &ObjectTracker::GetInstance().GetObjectByTag("agent");

		if (!pathStack.empty()) {
			MazeCell nextMove = pathStack.top();

			if (agent != nullptr) {
				if (AtNextPosition(agent->GetRigidBody()->box2dBody->GetPosition().x, agent->GetRigidBody()->box2dBody->GetPosition().y, nextMove, 0.005f)) {
					std::cout << pathStack.top().GetColumn() << "," << -pathStack.top().GetRow() << std::endl;
					pathStack.pop();
				}
				else {
					Move(nextMove.GetColumn(), -nextMove.GetRow());
				}

			}

		}
		else {
			
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
