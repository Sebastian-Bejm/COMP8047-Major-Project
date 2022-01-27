#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Agent.h"

#include "GameObject.h"
#include "ObjectTracker.h"
#include "Renderer.h"
#include "PhysicsWorld.h"
#include "MazeGenerator.h"

const int screenWidth = 1200;
const int screenHeight = 900;

const float timeStep = 1.0f / 60.0f;

ObjectTracker* objectTracker;
Renderer* renderer;
PhysicsWorld* physicsWorld;

MazeGenerator mazeGenerator;

// might have to regulate to a ShaderFactory?
Shader crateShader, brickShader;

Agent dummyAgent;

int Initialize() {
	renderer = Renderer::GetInstance();

	glm::fvec4 backgroundColour(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 1.0f);
	renderer->Init(backgroundColour, screenWidth, screenHeight);

	objectTracker = &(ObjectTracker::GetInstance());
	physicsWorld = &(PhysicsWorld::GetInstance());

	// Generate a maze of size m x n (medium/large size, use odd numbers)
	mazeGenerator.InitMaze(9, 9); // TODO: correctly render the maze
	mazeGenerator.Generate();

	Camera camera(screenWidth, screenHeight, glm::vec3(0.0f, 2.5f, 7.0f));
	renderer->SetCamera(camera);

	return 0;
}

void CreateScene() {

	crateShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");
	crateShader.Activate();

	brickShader = Shader("TextureVertShader.vs", "TextureFragShader.fs");
	brickShader.Activate();

	GameObject testCube("TestCube", "crate.jpg", ShapeType::CUBE, crateShader,
		glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	testCube.SetBodyType(b2_dynamicBody);

	objectTracker->Add(testCube);
	physicsWorld->AddObject(&testCube);

	// set up boxes to test collisions
	int pos = -1.0f;
	for (int i = 0; i < 4; i++) {
		GameObject box("WallCube", "brick.png", ShapeType::CUBE, brickShader,
			glm::vec3(pos, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		objectTracker->Add(box);
		physicsWorld->AddObject(&box);
		pos += 1.0f;
	}

	int y = 1.0f;
	for (int i = 0; i < 3; i++) {
		GameObject box("WallCube", "brick.png", ShapeType::CUBE, brickShader,
			glm::vec3(pos - 1.0f, y + 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		objectTracker->Add(box);
		physicsWorld->AddObject(&box);
		y += 1.0f;
	}
}


// https://www.youtube.com/watch?v=7D8lLbp9_rQ
void CreateMazeScene() {
	std::vector<std::vector<MazeCell>> maze = mazeGenerator.GetMazeCells();

	Shader wallShader("TextureVertShader.vs", "TextureFragShader.fs");
	wallShader.Activate();

	// Create the maze and center it via camera
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

				GameObject wall("WallCube", "brick.png", ShapeType::CUBE, wallShader,
					position, rotation, scale);
				objectTracker->Add(wall);
				physicsWorld->AddObject(&wall);
			}
		}
	}
}

void HandleInputs() {
	GameObject* go = &objectTracker->GetAllObjects()[0];

	float velX = 0.0f, velY = 0.0f;
	float speed = 0.25f;

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

	go->GetRigidBody()->box2dBody->SetLinearVelocity(b2Vec2(velX, velY));
}

void PhysicsUpdate() {
	//HandleInputs();
	//GameObject* go = &objectTracker->GetAllObjects()[0]; // crate

	//dummyAgent.Move(go, -2.0f, 1.0f);

	physicsWorld->Update(objectTracker);
}

void GraphicsUpdate() {
	renderer->Update(objectTracker);
}

int RunEngine() {

	// physics update comes first
	PhysicsUpdate();

	// graphics comes after physics
	GraphicsUpdate();

	return 0;
}

int Teardown() {

	// Deletes memory allocated by OpenGL that is stored in game objects
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
	CreateScene();
	//CreateMazeScene();

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