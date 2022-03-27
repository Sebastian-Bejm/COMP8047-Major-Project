#include "GameManager.h"

GameManager& GameManager::GetInstance() {
	static GameManager instance;
	return instance;
}

void GameManager::Attach(ObstructionGenerator* obsGenerator, MazeGenerator* mazeGenerator) {
	this->obsGenerator = obsGenerator;
	this->mazeGenerator = mazeGenerator;	
}

void GameManager::LoadScene() {

}

void GameManager::ResetScene() {

}

void GameManager::ClearScene() {

}

// Updates the game logic
void GameManager::Update() {
	GameObject* agent = &ObjectTracker::GetInstance().GetObjectByTag("agent");

	bool terminalState = InTerminalState(agent);
	if (terminalState) {
		resetGame = true;
	}
}

// Checks if the game is in a terminal state:
// The agent has reached the end goal, or the agent is stuck (should not happen but it is rare)
bool GameManager::InTerminalState(GameObject* agent) {
	std::vector<int> endPoint = mazeGenerator->GetEndCoordinates();

	RigidBody* agentRb = agent->GetRigidBody();
	float xPos = agentRb->box2dBody->GetPosition().x;
	float yPos = agentRb->box2dBody->GetPosition().y;

	int roundX = (int)round(xPos);
	int roundY = (int)round(yPos);

	if (endPoint[0] == roundX && endPoint[1] == roundY) {
		return true;
	}
	return false;
}