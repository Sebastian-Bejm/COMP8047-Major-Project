#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "GameObject.h"
#include "ObjectTracker.h"
#include "Renderer.h"

const int screenWidth = 1200;
const int screenHeight = 900;

const float timeStep = 1.0f / 60.0f;

float deltaTime;
float currentFrame, lastFrame;

ObjectTracker* objectTracker;
Renderer* renderer;

// TODO: refactor later for initalize, update, and teardown
int Initialize() {
	renderer = Renderer::GetInstance();

	glm::fvec4 backgroundColour(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 1.0f);
	renderer->Init(backgroundColour, screenWidth, screenHeight);

	objectTracker = ObjectTracker::GetInstance();

	Camera camera(screenWidth, screenHeight, glm::vec3(0.0f, 0.5f, 5.0f));
	renderer->SetCamera(camera);

	return 0;
}

void CreateScene() {
	int pos = -1.0f;
	for (int i = 0; i < 4; i++) {
		Shader cubeShader("TextureVertShader.vs", "TextureFragShader.fs");

		cubeShader.Activate();

		GameObject testCube("New", "brick.png", ShapeType::CUBE, cubeShader,
			glm::vec3(pos, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		//std::cout << pos << std::endl;
		objectTracker->Add(testCube);
		pos += 1.0f;
	}
}

void GraphicsUpdate() {
	//renderer->Update(objectTracker);
}

void PhysicsUpdate() {
	// run physics here
	// this update uses a fixed step
}

void HandleInputs() {
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		//objects[1].GetTransform().Translate(glm::vec3(-0.5f, 0.0f, 0.0f), deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		//objects[1].GetTransform().Translate(glm::vec3(0.5f, 0.0f, 0.0f), deltaTime);
	}
}

int RunEngine() {

	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// physics update comes first
	PhysicsUpdate();
	// graphics comes after physics
	GraphicsUpdate();

	return 0;
}

int Teardown() {
	// Deletes allocated memory stored in game objects
	objectTracker->DeleteAllObjects();

	// Destroys the window on exit
	renderer->Teardown();

	return 0;
}


int main() {

	// Initalize everything required for engine
	Initialize();

	CreateScene();

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		// This fixes objects/entities moving too fast
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Tell GLFW to keep track of input events
		glfwPollEvents();

		renderer->Update(objectTracker, deltaTime);
	}

	// Cleanup objects and destroy/exit window when done
	Teardown();

	return 0;
}