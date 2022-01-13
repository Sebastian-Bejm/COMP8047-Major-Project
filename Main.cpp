#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameObject.h"
#include "ObjectTracker.h"

const int screenWidth = 1000;
const int screenHeight = 800;

float deltaTime;
float currentFrame, lastFrame;

ObjectTracker* objectTracker;

// TODO: refactor later for initalize, update, and teardown
int Initialize() {

	glm::fvec4 backgroundColour(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 1.0f);


	return 0;
}

int Teardown() {
	return 0;
}



int main() {

	// Init GLFW
	glfwInit();

	// Tell GLFW what version we are using (OpenGL 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create GLFW window
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "COMP 8047 Major Project", NULL, NULL);
	// If window fails to create it exits
	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the window
	glViewport(0, 0, screenWidth, screenHeight);

	objectTracker = ObjectTracker::GetInstance();


	// Test adding
	int pos = -2.0f;
	for (int i = 0; i < 4; i++) {
		Shader cubeShader("TextureVertShader.vs", "TextureFragShader.fs");

		cubeShader.Activate();

		GameObject testCube("New", "brick.png", CUBE, cubeShader,
			glm::vec3(pos, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		std::cout << pos << std::endl;

		objectTracker->Add(testCube);
		pos += 1.0f;
	}

	// Specify the color of the background (silver)
	glClearColor(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	// Enable the depth buffer for 3D objects
	glEnable(GL_DEPTH_TEST);

	// Set up the camera
	Camera camera(screenWidth, screenHeight, glm::vec3(0.0f, 0.5f, 5.0f));

	std::vector<GameObject> objects = objectTracker->GetAllObjects();

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		// This fixes objects/entities moving too fast
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Specify the color of the background (silver)
		glClearColor(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.ProcessInput(window, deltaTime);
		camera.SetMatrix(45.0f, 0.1f, 100.0f);

		// Order of transforms once the matrices are set does not matter in the update
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			objects[1].GetTransform().Translate(glm::vec3(-0.5f, 0.0f, 0.0f), deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			objects[1].GetTransform().Translate(glm::vec3(0.5f, 0.0f, 0.0f), deltaTime);
		}

		for (int i = 0; i < objects.size(); i++) {
			objects[i].Draw(camera);
		}

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		// todo create Update functions for rendering, physics, manager, etc.
		glfwPollEvents();
	}

	// Cleanup objects we have created

	objectTracker->DeleteAllObjects();

	// Destroy window when done and exit
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}