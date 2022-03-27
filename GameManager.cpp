#include "GameManager.h"

GameManager& GameManager::GetInstance() {
	static GameManager instance;
	return instance;
}

void GameManager::Attach(ObstructionGenerator* obsGenerator, MazeGenerator* mazeGenerator) {
	this->obsGenerator = obsGenerator;
	this->mazeGenerator = mazeGenerator;	
}

// Initialize the shaders to be used
void GameManager::LoadShaders() {
	Shader crateShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");
	Shader brickShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");

	shaderStorage.push_back(crateShader);
	shaderStorage.push_back(brickShader);
}

void GameManager::LoadScene() {
	std::vector<std::vector<MazeCell>> maze = mazeGenerator->GetMazeCells();

	// Create the maze using game objects
	// Row: y, Col: x;
	for (size_t r = 0; r < maze.size(); r++) {
		for (size_t c = 0; c < maze[r].size(); c++) {
			// Create agent if cell is the start point
			if (maze[r][c].IsStart()) {
				int x = c;
				int y = r;

				glm::vec3 startPos = glm::vec3(x, -y, 0.0f);
				glm::vec3 agentRotation = glm::vec3(0.0f);
				glm::vec3 agentScale = glm::vec3(0.6f, 0.6f, 0.6f);

				// The agent is the first object added to the object tracker
				GameObject agent("agent", "crate.jpg", shaderStorage[0], startPos, agentRotation, agentScale);
				agent.SetBodyType(b2_dynamicBody);

				ObjectTracker::GetInstance().AddObject(agent);
				PhysicsWorld::GetInstance().AddObject(&agent);
			}

			// Only create objects when a maze cell is a wall
			if (maze[r][c].IsWall()) {
				int x = c;
				int y = r;

				glm::vec3 position = glm::vec3(x, -y, 0.0f);
				glm::vec3 rotation = glm::vec3(0.0f);
				glm::vec3 scale = glm::vec3(1.0f);

				GameObject wall("wall", "brick.png", shaderStorage[1], position, rotation, scale);

				ObjectTracker::GetInstance().AddObject(wall);
				PhysicsWorld::GetInstance().AddObject(&wall);
			}
		}
	}
}

void GameManager::ResetScene() {
	// reset the scene by setting everything back in original positions
	// and removing obstructions (or have a reset function in obstruction generator)
}

void GameManager::ClearScene() {
	// clear the whole scene to prepare for a new scene
	// temp: deleting shaders
	for (auto& shader : shaderStorage) {
		shader.Delete();
	}
}

// Updates the game logic and checks when the game has finished
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