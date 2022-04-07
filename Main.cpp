#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "GameObject.h"
#include "ObjectTracker.h"
#include "Renderer.h"
#include "PhysicsWorld.h"
#include "MazeGenerator.h"
#include "ObstructionGenerator.h"
#include "GameManager.h"
#include "Agent.h"
#include "FPSCounter.h"
#include "QLearn.h"

const int screenWidth = 1200;
const int screenHeight = 1000;

const int mazeRows = 13, mazeCols = 13;
const int cameraDepth = 19.0f;

ObjectTracker* objectTracker;
Renderer* renderer;
PhysicsWorld* physicsWorld;
MazeGenerator mazeGenerator;
ObstructionGenerator obsGenerator;
GameManager* gameManager;

Camera camera;

int Initialize() {
	glm::fvec4 backgroundColour(180.0f / 255.0f, 240.0f / 255.0f, 239.0f / 255.0f, 1.0f);
	renderer = Renderer::GetInstance();
	renderer->Init(backgroundColour, screenWidth, screenHeight);

	objectTracker = &ObjectTracker::GetInstance();
	physicsWorld = &PhysicsWorld::GetInstance();

	// Generate a maze of size m x n (use odd numbers to avoid wall issue)
	mazeGenerator.InitMaze(mazeRows, mazeCols);
	mazeGenerator.Generate();
	//mazeGenerator.InitMaze("maze.txt");

	obsGenerator.AttachMazeGenerator(&mazeGenerator);

	gameManager = &GameManager::GetInstance();
	gameManager->Attach(&obsGenerator, &mazeGenerator);
	gameManager->LoadShaders();

	camera = Camera(screenWidth, screenHeight, glm::vec3((float)(mazeCols/2), (float)(-mazeRows/2), cameraDepth));
	renderer->SetCamera(camera);

	return 0;
}

// This will remain here until everything is finished and Agent properly works
void HandleInputs() {
	// REMOVE OBJECTS THROUGH RemoveAllObjects() this way:
	/*if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		objectTracker->RemoveAllObjects();
		objectTracker->DeleteAllObjects();
		gameManager->LoadNewScene();
	}*/

	/*GameObject* agent = &objectTracker->GetObjectByTag("agent");

	float velX = 0.0f, velY = 0.0f;
	float speed = 0.35f;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		velX = -speed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		velX = speed;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		velY = speed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		velY = -speed;
	}

	if (agent != nullptr) {
		agent->SetVelocity(velX, velY);
	}*/
}

void PhysicsUpdate() {
	HandleInputs();

	physicsWorld->Update(objectTracker);
}

void GraphicsUpdate() {
	renderer->Update(objectTracker);
}

int RunEngine() {
	// physics update comes first
	PhysicsUpdate();

	//agent.MoveUpdate();
	gameManager->Update();

	// graphics comes after physics
	GraphicsUpdate();

	return 0;
}

// Delete objects and free any memory that has been used
int Teardown() {

	// Deletes pointers that is stored in game objects
	objectTracker->DeleteAllObjects();
	
	// Clean up the scene and delete all objects
	gameManager->CleanScene();

	// Destroys the window on exit
	renderer->Teardown();

	return 0;
}

void ReadCSV(std::string filename, std::vector<Eigen::RowVectorXf>& data) {
	data.clear();
	std::ifstream file(filename);
	std::string line, word;
	// determine number of columns in file
	std::getline(file, line, '\n');
	std::stringstream ss(line);
	std::vector<float> parsed_vec;
	
	while (std::getline(ss, word, ',')) {
		parsed_vec.push_back(float(std::stof(&word[0])));
	}
	unsigned int cols = parsed_vec.size();
	data.push_back(Eigen::RowVectorXf(cols));
	for (int i = 0; i < cols; i++) {
		data.back().coeffRef(1, i) = parsed_vec[i];
	}

	// read the file
	if (file.is_open()) {
		while (std::getline(file, line, '\n')) {
			std::stringstream ss(line);
			data.push_back(Eigen::RowVectorXf(1, cols));
			unsigned int i = 0;
			while (std::getline(ss, word, ',')) {
				data.back().coeffRef(i) = float(std::stof(&word[0]));
				i++;
			}
		}
	}
}

void GenData(std::string filename) {
	std::ofstream file1(filename + "-in");
	std::ofstream file2(filename + "-out");
	for (int r = 0; r < 1000; r++) {
		float x = rand() / float(RAND_MAX);
		float y = rand() / float(RAND_MAX);
		file1 << x << "," << y << std::endl;
		file2 << 2 * x + 10 + y << std::endl;
	}
	file1.close();
	file2.close();
}

int main() {
	// TODO: 
	// ObstructionGenerator => change so it generates walls that are marked as the obstructions
	// ELM in Q-Learning
	// Proper updating: Get Maze => Agent => Train => Move => if maze is updated then repeat

	Agent agent = Agent();

	agent.InitializeQLearn();
	agent.Train(Mode::QLEARN, false);

	// Initalize everything required for engine
	Initialize();

	// Load the initial scene
	gameManager->LoadScene();

	// Main loop
	while (!glfwWindowShouldClose(window)) {

		// Tell GLFW to keep track of input events
		glfwPollEvents();

		// Run the engine and its updates
		PhysicsUpdate();
		agent.MoveUpdate();
		gameManager->Update();

		GraphicsUpdate();
	}

	// Cleanup objects and destroy/exit window when done
	Teardown();

	return 0;
}