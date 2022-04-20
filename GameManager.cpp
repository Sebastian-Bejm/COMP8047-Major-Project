#include "GameManager.h"

GameManager& GameManager::GetInstance() {
	static GameManager instance;
	return instance;
}

// Initialize the shaders to be used
void GameManager::LoadShaders() {
	Shader crateShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");
	Shader brickShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");

	shaderStorage.push_back(crateShader);
	shaderStorage.push_back(brickShader);
}

// Load a scene given a generated maze, and position the rendered objects in the scene
void GameManager::LoadScene() {
	std::vector<std::vector<MazeCell>> maze = MazeGenerator::GetInstance().GetMazeCells();
	ObjectTracker* trackerInstance = &ObjectTracker::GetInstance();
	PhysicsWorld* worldInstance = &PhysicsWorld::GetInstance();

	// Create the maze using game objects
	// Row: -y, Col: x;
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

				trackerInstance->AddObject(agent);
				worldInstance->AddObject(&agent);

				// Render an object representing the entrance
				glm::vec3 startPointPos = glm::vec3(x, -y, -5.0f);
				GameObject startingPoint("point", "start_tex.jpg", shaderStorage[0], startPointPos, rotation, generalScale);

				trackerInstance->AddObject(startingPoint);
				worldInstance->AddObject(&startingPoint);
			}

			// Render an object representing the exit
			if (maze[r][c].IsExit()) {
				glm::vec3 endPointPos = glm::vec3(x, -y, -5.0f);
				GameObject endingPoint("point", "end_tex.jpg", shaderStorage[0], endPointPos, rotation, generalScale);

				trackerInstance->AddObject(endingPoint);
				worldInstance->AddObject(&endingPoint);
			}

			// Create the wall objects
			if (maze[r][c].IsWall()) {
				glm::vec3 position = glm::vec3(x, -y, 0.0f);

				GameObject wall("wall", "brick.png", shaderStorage[1], position, rotation, generalScale);

				trackerInstance->AddObject(wall);
				worldInstance->AddObject(&wall);
			}
		}
	}

	// Start timer and generator
	TimeTracker::GetInstance().StartTimer();
	ObstructionGenerator::GetInstance().StartGenerator(true);

	pathfindingAgent.InitializeQLearn();
	pathfindingAgent.InitializeEnvironment();
	pathfindingAgent.Train(Mode::QLEARN);
}

// Updates the game logic and checks when the current game has finished
void GameManager::Update() {
	GameObject* agent = &ObjectTracker::GetInstance().GetObjectByTag("agent");
	ObstructionGenerator* generatorInstance = &ObstructionGenerator::GetInstance();

	if (generatorInstance->GetMazeUpdates()) {
		// TODO: MAJOR MEMORY ISSUE!
		// 
		glm::vec3 curPos = agent->GetTransform()->GetPosition();
		pathfindingAgent.UpdateCurrentState(curPos);
		pathfindingAgent.Train(Mode::QLEARN);
	}

	pathfindingAgent.MoveUpdate();

	if (agent != nullptr) {
		bool terminalState = InTerminalState(agent);

		if (terminalState) {
			TimeTracker::GetInstance().StopTimer();
			reachedGoal = true;
		}

		if (reachedGoal) {
			timeAfterGoal++;

			if (timeAfterGoal >= graceTime) {
				CleanScene();

				LoadNewScene();

				generatorInstance->StartGenerator(true);

				mazesCompleted++;
				timeAfterGoal = 0;
				reachedGoal = false;
			}
		}

	}

}

// Loads a new scene: generates a new maze, start/end point, and agent in new positions
void GameManager::LoadNewScene() {
	MazeGenerator::GetInstance().InitMaze(15, 15, 50);
	MazeGenerator::GetInstance().Generate();

	LoadScene();
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
void GameManager::CleanScene() {
	PhysicsWorld::GetInstance().DestroyObjects();

	ObjectTracker::GetInstance().DeleteAllObjects();
	ObjectTracker::GetInstance().RemoveAllObjects();

	/*for (auto& shader : shaderStorage) {
		shader.Delete();
	}*/
}

int GameManager::GetMazesCompleted() {
	return mazesCompleted;
}

// Checks if the game is in a terminal state:
// The agent has reached the end goal, or the agent is stuck (should not happen but it is rare)
bool GameManager::InTerminalState(GameObject* agent) {
	MazeCell endPoint = MazeGenerator::GetInstance().GetEndCell();

	RigidBody* agentRb = agent->GetRigidBody();
	float xPos = agentRb->box2dBody->GetPosition().x;
	float yPos = agentRb->box2dBody->GetPosition().y;

	int roundX = (int)round(xPos);
	int roundY = (int)round(yPos);

	if (!reachedGoal) {
		if (endPoint.GetColumn() == roundX && -endPoint.GetRow() == roundY) {
			return true;
		}
	}
	return false;
}