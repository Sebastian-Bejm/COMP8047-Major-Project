#include "ObstructionGenerator.h"

// Returns the singleton instance of this class
ObstructionGenerator& ObstructionGenerator::GetInstance() {
	static ObstructionGenerator instance;
	return instance;
}

// Perform updates in the generator
void ObstructionGenerator::Update() {
	if (generatorStarted) {
		FindObstructions();
		generatorStarted = false;
	}

	if (InRange()) {
		mazeUpdated = true;
		UpdateMarkedObstructions();
	}
}

// If obstructions are marked, remove them from vector
void ObstructionGenerator::UpdateMarkedObstructions() {
	obstructions.erase(std::remove_if(
		obstructions.begin(), obstructions.end(),
		[](MazeCell& mc) {
			return mc.IsWall();
	}), obstructions.end());
}

// Starts the generator, used when we need to get new obstructions
void ObstructionGenerator::StartGenerator(bool start) {
	generatorStarted = start;
}

// Get the last update from when the maze was updated with obstructions
bool ObstructionGenerator::GetMazeUpdates() {
	bool currentMazeUpdated = mazeUpdated;
	mazeUpdated = false;
	return currentMazeUpdated;
}

// Get marked obstructions from the maze generator
void ObstructionGenerator::FindObstructions() {
	MazeGenerator* mazeGenInstance = &MazeGenerator::GetInstance();
	std::vector<std::vector<MazeCell>> maze = mazeGenInstance->GetMazeCells();

	obstructions.clear(); // make sure the obstructions are all cleared

	for (size_t i = 0; i < maze.size(); i++) {
		for (size_t j = 0; j < maze[i].size(); j++) {
			if (maze[i][j].IsObstruction()) {
				obstructions.push_back(maze[i][j]);
			}
		}
	}

}

// Generate an obstruction within a maze cell
void ObstructionGenerator::GenerateObstruction(MazeCell obsCell) {
	Shader lavaShader = Shader(TEXTURE_VS, TEXTURE_FS);

	glm::vec3 obsPos = glm::vec3((float)obsCell.GetColumn(), (float)(-obsCell.GetRow()), 0.0f);
	GameObject obstruction("obstruction", LAVA_TEX, lavaShader, obsPos, glm::vec3(0.0f), glm::vec3(1.0f));

	ObjectTracker::GetInstance().AddObject(obstruction);
	PhysicsWorld::GetInstance().AddObject(&obstruction);
}

// Check if the agent is within range of an obstruction and mark them as walls
bool ObstructionGenerator::InRange() {
	GameObject* agent = &ObjectTracker::GetInstance().GetObjectByTag("agent");
	MazeGenerator* mazeGenInstance = &MazeGenerator::GetInstance();

	for (size_t i = 0; i < obstructions.size(); i++) {
		// Check if agent is within radius of a hidden obstruction
		MazeCell obstacle = obstructions[i];
		int obsX = obstacle.GetColumn();
		int obsY = -obstacle.GetRow();

		if (agent != nullptr) {
			RigidBody* agentRb = agent->GetRigidBody();
			float xPos = agentRb->box2dBody->GetPosition().x;
			float yPos = agentRb->box2dBody->GetPosition().y;

			// Check x and y radius range
			glm::vec2 agentPos = glm::vec2(xPos, yPos);
			glm::vec2 obsPos = glm::vec2(obsX, obsY);
			if (glm::distance(agentPos, obsPos) <= revealRadius) {
				//std::cout << glm::distance(agentPos, obsPos) << std::endl;
				obstructions[i].SetWall(true);
				mazeGenInstance->GetMazeCells()[obstacle.GetRow()][obstacle.GetColumn()].SetWall(true);

				GenerateObstruction(obstacle);
				return true;
			}

		}
	}

	return false;
}
