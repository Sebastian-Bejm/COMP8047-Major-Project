#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "GameObject/GameObject.h"
#include "MainSystems/ObjectTracker.h"
#include "Rendering/Renderer.h"
#include "Physics/PhysicsWorld.h"
#include "MainSystems/MazeGenerator.h"
#include "MainSystems/ObstructionGenerator.h"
#include "MainSystems/GameManager.h"
#include "AI/Agent.h"
#include "Utility/FPSCounter.h"
#include "AI/QLearn.h"
#include "AI/SampleData.h"

const int windowWidth = 1200, windowHeight = 980;
const int mazeRows = 15, mazeCols = 15;
const int wallToRemove = 50;

const float viewBounds[] = { -9.0f, 10.0f, -9.0f, 9.0f };

ObjectTracker* objectTracker;
Renderer* renderer;
PhysicsWorld* physicsWorld;
MazeGenerator* mazeGenerator;
ObstructionGenerator* obsGenerator;
GameManager* gameManager;

Camera camera;
FPSCounter counter = FPSCounter();

// Initialize all systems in the engine
int Initialize() {
	glm::fvec4 backgroundColour(180.0f / 255.0f, 240.0f / 255.0f, 239.0f / 255.0f, 1.0f);

	renderer = Renderer::GetInstance();
	renderer->Init(backgroundColour, windowWidth, windowHeight);
	renderer->SetView(viewBounds[0], viewBounds[1], viewBounds[2], viewBounds[3]);

	objectTracker = &ObjectTracker::GetInstance();
	physicsWorld = &PhysicsWorld::GetInstance();

	// Generate a maze of size m x n (use odd numbers to avoid wall issue)
	mazeGenerator = &MazeGenerator::GetInstance();
	mazeGenerator->InitMaze(mazeRows, mazeCols, wallToRemove);
	mazeGenerator->Generate();

	obsGenerator = &ObstructionGenerator::GetInstance();

	gameManager = &GameManager::GetInstance();
	gameManager->LoadShaders();

	camera = Camera(windowWidth, windowHeight, glm::vec3((float)(mazeCols/2), (float)(-mazeRows/2), 0));
	renderer->SetCamera(camera);

	return 0;
}

void PhysicsUpdate() {
	physicsWorld->Update(objectTracker);
}

void GraphicsUpdate() {
	renderer->Update(objectTracker);
}

int RunEngine() {
	// Physics update comes first
	PhysicsUpdate();

	gameManager->Update();

	// graphics comes after physics
	GraphicsUpdate();

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

		// FPS counter
		//counter.NextFrame();

		// Run the engine systems and its updates
		PhysicsUpdate();
		gameManager->Update();
		obsGenerator->Update(); 
		GraphicsUpdate();
	}

	// Cleanup objects and destroy/exit window when done
	Teardown();

	return 0;
}