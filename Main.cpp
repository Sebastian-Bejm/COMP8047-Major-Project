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

const int screenWidth = 1200;
const int screenHeight = 900;

const float timeStep = 1.0f / 60.0f;

ObjectTracker* objectTracker;
Renderer* renderer;
PhysicsWorld* physicsWorld;
MazeGenerator mazeGenerator;
ObstructionGenerator obsGenerator;
GameManager* gameManager;

Camera camera;
Shader crateShader, brickShader;

Agent randomAgent;

int Initialize() {
	glm::fvec4 backgroundColour(180.0f / 255.0f, 240.0f / 255.0f, 239.0f / 255.0f, 1.0f);
	renderer = Renderer::GetInstance();
	renderer->Init(backgroundColour, screenWidth, screenHeight);

	objectTracker = &ObjectTracker::GetInstance();
	physicsWorld = &PhysicsWorld::GetInstance();

	// Generate a maze of size m x n (medium/large size, use odd numbers)
	mazeGenerator.InitMaze(15, 15);
	mazeGenerator.Generate();

	obsGenerator.AttachMazeGenerator(&mazeGenerator);

	gameManager = &GameManager::GetInstance();
	gameManager->Attach(&obsGenerator, &mazeGenerator);

	camera = Camera(screenWidth, screenHeight, glm::vec3(6.5f, -6.5f, 19.0f));
	renderer->SetCamera(camera);

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		
	}
}

// Load the shaders to be used for objects in the scene
void LoadShaders() {
	crateShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");
	brickShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");
}

// Create the scene that includes the maze and agent object at the start
void CreateMazeScene() {

	std::vector<std::vector<MazeCell>> maze = mazeGenerator.GetMazeCells();
	//mazeGenerator.PrintMaze();

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
				GameObject agent("agent", "crate.jpg", crateShader, startPos, agentRotation, agentScale);
				agent.SetBodyType(b2_dynamicBody);

				objectTracker->AddObject(agent);
				physicsWorld->AddObject(&agent);

				randomAgent.AttachAgentObject(&agent);
			}

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

// This will remain here until everything is finished and Agent properly works
void HandleInputs() {
	GameObject* agent = &objectTracker->GetObjectByTag("agent");

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

	agent->GetRigidBody()->box2dBody->SetLinearVelocity(b2Vec2(velX, velY));
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
	randomAgent.MoveUpdate();

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
	LoadShaders();

	// Load the initial scene
	// GameManager.LoadScene
	CreateMazeScene();

	// Main loop
	while (!glfwWindowShouldClose(window)) {

		// Tell GLFW to keep track of input events
		glfwPollEvents();

		//RunEngine();
		PhysicsUpdate();
		gameManager->Update();

		GraphicsUpdate();
	}

	// Cleanup objects and destroy/exit window when done
	Teardown();

	return 0;
}