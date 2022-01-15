#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "GameObject.h"
#include "ObjectTracker.h"
#include "Renderer.h"
#include "PhysicsWorld.h"

const int screenWidth = 1200;
const int screenHeight = 900;

const float timeStep = 1.0f / 60.0f;

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

	Shader cubeShader("TextureVertShader.vs", "TextureFragShader.fs");

	cubeShader.Activate();
	GameObject testCube("WallCube", "crate.jpg", ShapeType::CUBE, cubeShader,
		glm::vec3(-1.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	objectTracker->Add(testCube);

	Shader floorShader("FloorVertShader.vs", "FloorFragShader.fs");

	floorShader.Activate();
	GameObject floor("Floor", "brick.png", ShapeType::CUBE, floorShader,
		glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5.0f, 1.0f, 5.0f));
	objectTracker->Add(floor);

	/*int pos = -1.0f;
	for (int i = 0; i < 4; i++) {
		Shader cubeShader("TextureVertShader.vs", "TextureFragShader.fs");

		cubeShader.Activate();

		GameObject testCube("WallCube", "crate.jpg", ShapeType::CUBE, cubeShader,
			glm::vec3(pos, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		//std::cout << pos << std::endl;
		objectTracker->Add(testCube);
		pos += 1.0f;
	}*/
}

void GraphicsUpdate() {
	renderer->Update(objectTracker);
}

void PhysicsUpdate() {
	// run physics here
	// this update uses a fixed step
	// physics->Update
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

	// Create a scene for the purposes of testing
	CreateScene();

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