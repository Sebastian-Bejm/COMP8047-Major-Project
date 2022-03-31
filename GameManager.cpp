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
			int x = c;
			int y = r;
			// rotation stays the same for each object
			glm::vec3 rotation = glm::vec3(0.0f);
			glm::vec3 generalScale = glm::vec3(1.0f);

			// Create agent if cell is the start point
			if (maze[r][c].IsStart()) {
				glm::vec3 startPos = glm::vec3(x, -y, 0.0f);
				// Agent scale is different so it doesn't get stuck in the maze
				glm::vec3 agentScale = glm::vec3(0.6f, 0.6f, 1.0f);

				// The agent is the first object added to the object tracker
				GameObject agent("agent", "crate.jpg", shaderStorage[0], startPos, rotation, agentScale);
				agent.SetBodyType(b2_dynamicBody);

				ObjectTracker::GetInstance().AddObject(agent);
				PhysicsWorld::GetInstance().AddObject(&agent);

				// Render an object representing the entrance
				glm::vec3 startPointPos = glm::vec3(x, -y, -1.0f);
				GameObject startingPoint("point", "crate.jpg", shaderStorage[0], startPointPos, rotation, generalScale);

				ObjectTracker::GetInstance().AddObject(startingPoint);
				PhysicsWorld::GetInstance().AddObject(&startingPoint);
			}

			// Render an object representing the exit
			if (maze[r][c].IsExit()) {
				glm::vec3 endPointPos = glm::vec3(x, -y, -1.0f);
				GameObject endingPoint("point", "crate.jpg", shaderStorage[0], endPointPos, rotation, generalScale);

				ObjectTracker::GetInstance().AddObject(endingPoint);
				PhysicsWorld::GetInstance().AddObject(&endingPoint);
			}

			// Create the wall objects
			if (maze[r][c].IsWall()) {
				glm::vec3 position = glm::vec3(x, -y, 0.0f);

				GameObject wall("wall", "brick.png", shaderStorage[1], position, rotation, generalScale);

				ObjectTracker::GetInstance().AddObject(wall);
				PhysicsWorld::GetInstance().AddObject(&wall);
			}
		}
	}

	// start the timer for this run
	TimeTracker::GetInstance().StartTimer();
}

// Resets the scene by resetting the Agent to its original location, and removes obstructions
void GameManager::ResetScene() {
	GameObject* agent = &ObjectTracker::GetInstance().GetObjectByTag("agent");
	agent->ResetTransform();

	reachedGoal = false;
	timeAfterGoal = 0;
	TimeTracker::GetInstance().StartTimer();
}

// Clear the scene and properly delete the objects currently in the scene
void GameManager::ClearScene() {
	// clear the whole scene to prepare for a new scene
	
	// temp: deleting shaders
	for (auto& shader : shaderStorage) {
		shader.Delete();
	}
}

// Updates the game logic and checks when the current game has finished
void GameManager::Update() {
	GameObject* agent = &ObjectTracker::GetInstance().GetObjectByTag("agent");

	bool terminalState = InTerminalState(agent);
	if (terminalState) {
		TimeTracker::GetInstance().StopTimer();
		reachedGoal = true;	
	}
	if (reachedGoal) {
		timeAfterGoal++;
		if (timeAfterGoal >= graceTime) {
			ResetScene();
		}
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

	if (!reachedGoal) {
		if (endPoint[0] == roundX && endPoint[1] == roundY) {
			return true;
		}
	}
	return false;
}