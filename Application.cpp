#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Rendering/Renderer.h"
#include "Physics/PhysicsWorld.h"
#include "Utility/FPSCounter.h"
#include "AI/Agent.h"
#include "AI/QLearn.h"
#include "AI/SampleData.h"

ObjectTracker* objectTracker;
Renderer* renderer;
PhysicsWorld* physicsWorld;
MazeGenerator* mazeGenerator;
ObstructionGenerator* obsGenerator;
GameManager* gameManager;

// Initialize all systems in the engine
int Initialize() {
	glm::fvec4 backgroundColour(180.0f / 255.0f, 240.0f / 255.0f, 239.0f / 255.0f, 1.0f);

	renderer = Renderer::GetInstance();
	renderer->Init(backgroundColour, WINDOW_WIDTH, WINDOW_HEIGHT);
	renderer->SetView(VIEW_BOUNDS[0], VIEW_BOUNDS[1], VIEW_BOUNDS[2], VIEW_BOUNDS[3]);

	objectTracker = &ObjectTracker::GetInstance();
	physicsWorld = &PhysicsWorld::GetInstance();

	// Generate a maze of size m x n (use odd numbers to avoid wall issue)
	mazeGenerator = &MazeGenerator::GetInstance();
	mazeGenerator->InitMaze(MAZE_ROWS, MAZE_COLS, WALLS_TO_REMOVE);
	mazeGenerator->Generate();

	obsGenerator = &ObstructionGenerator::GetInstance();

	gameManager = &GameManager::GetInstance();
	gameManager->LoadShaders();

	float centerX = (float)(MAZE_COLS / 2);
	float centerY = (float)(-MAZE_ROWS / 2);

	Camera camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(centerX, centerY, 0));
	renderer->SetCamera(camera);

	return 0;
}

// Run the engine and its systems updates
int RunEngine() {

	gameManager->Update();
	obsGenerator->Update();

	// Physics update comes first
	physicsWorld->Update(objectTracker);

	// graphics comes after physics
	renderer->Update(objectTracker);

	return 0;
}

// Delete objects and free any memory that has been used
int Teardown() {	
	// Clean up the scene and delete all objects
	gameManager->CleanScene(true);

	// Destroys the window on exit
	renderer->Teardown();

	return 0;
}

int main() {

	// Initalize everything required for engine
	Initialize();

	// Load the initial scene
	gameManager->LoadScene();

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		// Tell GLFW to keep track of input events
		glfwPollEvents();

		// Run the engine systems and its updates
		RunEngine();
	}

	// Cleanup objects and destroy/exit window when done
	Teardown();

	return 0;
}