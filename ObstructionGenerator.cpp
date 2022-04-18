#include "ObstructionGenerator.h"

ObstructionGenerator& ObstructionGenerator::GetInstance() {
	static ObstructionGenerator instance;
	return instance;
}

void ObstructionGenerator::Update() {
	if (newMaze) {
		FindObstructions();
		newMaze = false;
	}

	// if is range and has already been updated, pass update to game manager
	if (InRange()) {
		
	}
}

void ObstructionGenerator::UpdateMarkedObstructions() {
	// TODO: If an obstruction has been marked as a wall, remove it from vector
	
}

void ObstructionGenerator::FindObstructions() {
	MazeGenerator* mazeGenInstance = &MazeGenerator::GetInstance();
	std::vector<std::vector<MazeCell>> maze = mazeGenInstance->GetMazeCells();

	for (size_t i = 0; i < maze.size(); i++) {
		for (size_t j = 0; j < maze[i].size(); j++) {
			int x = static_cast<int>(j);
			int y = -static_cast<int>(i);
			if (maze[i][j].IsObstruction()) {
				obstructions.push_back(maze[i][j]);
			}

		}
	}

}

void ObstructionGenerator::GenerateObstruction(MazeCell obsCell) {
	Shader lavaShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");

	glm::vec3 obsPos = glm::vec3((float)obsCell.GetColumn(), (float)(-obsCell.GetRow()), 0.0f);
	GameObject obstruction("obstruction", "lava.png", lavaShader, obsPos, glm::vec3(0.0f), glm::vec3(1.0f));

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

			// If the current obstruction has been marked as wall again, continue
			if (mazeGenInstance->GetMazeCells()[obstacle.GetRow()][obstacle.GetColumn()].IsWall()) {
				continue;
			}

			// Check x and y radius range
			if (fabs(xPos - obsX) <= revealRadius && fabs(yPos - obsY) <= revealRadius) {
				std::cout << obsX << "," << obsY << std::endl;
				mazeGenInstance->GetMazeCells()[obstacle.GetRow()][obstacle.GetColumn()].SetWall(true);
				//mazeGenInstance->PrintMaze();

				GenerateObstruction(obstacle);

				return true;
			}
		}
	}

	return false;
}
