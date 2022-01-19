#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

int Initialize() {
	renderer = Renderer::GetInstance();

	glm::fvec4 backgroundColour(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 1.0f);
	renderer->Init(backgroundColour, screenWidth, screenHeight);

	objectTracker = &(ObjectTracker::GetInstance());
	physicsWorld = &(PhysicsWorld::GetInstance());

	// Generate a maze of size m x n, use prime numbers
	MazeGenerator generator(25, 25);
	generator.Generate();

	Camera camera(screenWidth, screenHeight, glm::vec3(0.0f, 2.5f, 7.0f));
	renderer->SetCamera(camera);

	return 0;
}

void CreateScene() {

	Shader cubeShader("TextureVertShader.vs", "TextureFragShader.fs");

	cubeShader.Activate();
	GameObject testCube("TestCube", "crate.jpg", ShapeType::CUBE, cubeShader,
		glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	testCube.SetBodyType(b2_dynamicBody);
	objectTracker->Add(testCube);
	physicsWorld->AddObject(&testCube);

	// set up boxes to test collisions
	int pos = -1.0f;
	for (int i = 0; i < 4; i++) {
		Shader cubeShader2("TextureVertShader.vs", "TextureFragShader.fs");

		cubeShader2.Activate();

		GameObject box("WallCube", "brick.png", ShapeType::CUBE, cubeShader2,
			glm::vec3(pos, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		objectTracker->Add(box);
		physicsWorld->AddObject(&box);
		pos += 1.0f;
	}

	int z = 1.0f;
	for (int i = 0; i < 3; i++) {
		Shader cubeShader3("TextureVertShader.vs", "TextureFragShader.fs");

		cubeShader3.Activate();

		GameObject box("WallCube", "brick.png", ShapeType::CUBE, cubeShader3,
			glm::vec3(pos - 1.0f, z + 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		objectTracker->Add(box);
		physicsWorld->AddObject(&box);
		z += 1.0f;
	}
}

void HandleInputs() {
	GameObject* go = &objectTracker->GetAllObjects()[0];

	float velX = 0.0f, velY = 0.0f;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		velX = -0.8f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		velX = 0.8f;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		velY = 0.8f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		velY = -0.8f;
	}

	go->GetRigidBody()->box2dBody->SetLinearVelocity(b2Vec2(velX, velY));
}

void GraphicsUpdate() {
	renderer->Update(objectTracker);
}

void PhysicsUpdate() {
	//HandleInputs();
	physicsWorld->Update(objectTracker);
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

	// Destroys the window on exit
	renderer->Teardown();

	return 0;
}

int main() {

	// Initalize everything required for engine
	Initialize();

	// Create a scene for the purposes of testing
	//CreateScene();

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