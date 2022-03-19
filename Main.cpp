#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "GameObject.h"
#include "ObjectTracker.h"
#include "Renderer.h"
#include "PhysicsWorld.h"
#include "MazeGenerator.h"
#include "ObstructionGenerator.h"

#include "Agent.h"
#include "FPSCounter.h"

const int screenWidth = 1200;
const int screenHeight = 900;

const float timeStep = 1.0f / 60.0f;

ObjectTracker* objectTracker;
Renderer* renderer;
PhysicsWorld* physicsWorld;
MazeGenerator mazeGenerator;
//ObstructionGenerator obsGenerator;

Camera camera;
Shader crateShader, brickShader;

Agent randomAgent;

int Initialize() {
	renderer = Renderer::GetInstance();

	glm::fvec4 backgroundColour(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 1.0f);
	renderer->Init(backgroundColour, screenWidth, screenHeight);

	objectTracker = &(ObjectTracker::GetInstance());
	physicsWorld = &(PhysicsWorld::GetInstance());

	// Generate a maze of size m x n (medium/large size, use odd numbers)
	mazeGenerator.InitMaze(15, 15);
	mazeGenerator.Generate();

	//obsGenerator.AttachMaze(mazeGenerator.GetMazeCells());

	camera = Camera(screenWidth, screenHeight, glm::vec3(6.5f, -6.5f, 19.0f));
	renderer->SetCamera(camera);

	return 0;
}

// Load the shaders to be used for objects in the scene
void LoadShaders() {
	crateShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");
	brickShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");
}

// Create the scene which includes the generated maze as well as the agent object
void CreateMazeScene() {

	LoadShaders();

	std::vector<std::vector<MazeCell>> maze = mazeGenerator.GetMazeCells();
	mazeGenerator.PrintMaze();

	// Set the agent object at the starting cell
	std::vector<int> startCell = mazeGenerator.GetStartCoordinates();
	int startX = startCell[0];
	int startY = -startCell[1];

	glm::vec3 startPos = glm::vec3(startX, startY, 0.0f);
	glm::vec3 agentScale = glm::vec3(0.6f, 0.6f, 0.6f);

	// The agent is the first object added to the object tracker
	GameObject agent("agent", "crate.jpg", crateShader, startPos, glm::vec3(0.0f, 0.0f, 0.0f), agentScale);
	agent.SetBodyType(b2_dynamicBody);

	objectTracker->AddObject(agent);
	physicsWorld->AddObject(&agent);

	// Create the maze using game objects
	// Row: y, Col: x;
	for (size_t r = 0; r < maze.size(); r++) {
		for (size_t c = 0; c < maze[r].size(); c++) {

			// Only create objects when a maze cell is a wall
			if (maze[r][c].IsWall()) {
				int x = c;
				int y = r;

				glm::vec3 position = glm::vec3(x, -y, 0.0f);
				glm::vec3 rotation = glm::vec3(0.0f);
				glm::vec3 scale = glm::vec3(1.0f);

				GameObject wall("wall", "brick.png", brickShader, position, rotation, scale);

				objectTracker->AddObject(wall);
				physicsWorld->AddObject(&wall);
			}
		}
	}

}

void HandleInputs() {
	GameObject* go = &objectTracker->GetAllObjects().front();

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

	// spawn test


	go->GetRigidBody()->box2dBody->SetLinearVelocity(b2Vec2(velX, velY));
}

void PhysicsUpdate() {
	HandleInputs();

	physicsWorld->Update(objectTracker);
}

void GraphicsUpdate() {
	renderer->Update(objectTracker);
}

int RunEngine() {
	//obsGenerator.RunGenerator(objectTracker, physicsWorld);
	//randomAgent.Move(agentObject, 0, 0);

	// physics update comes first
	PhysicsUpdate();

	// graphics comes after physics
	GraphicsUpdate();

	return 0;
}

int Teardown() {

	// Deletes pointers that is stored in game objects
	objectTracker->DeleteAllObjects();
	
	// Delete shaders used
	crateShader.Delete();
	brickShader.Delete();

	// Destroys the window on exit
	renderer->Teardown();

	return 0;
}

int main() {

	// Initalize everything required for engine
	Initialize();

	// Create a scene for the purposes of testing
	CreateMazeScene();

	// Main loop
	while (!glfwWindowShouldClose(window)) {

		// Tell GLFW to keep track of input events
		glfwPollEvents();

		RunEngine();
	}

	// Cleanup objects and destroy/exit window when done
	Teardown();

	return 0;
}