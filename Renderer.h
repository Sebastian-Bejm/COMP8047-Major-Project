#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Camera.h"
#include "ObjectTracker.h"


class Renderer {
public:

	static Renderer* GetInstance();

	int Init(glm::vec4 backgroundColor);
	int Update(ObjectTracker* tracker); // ObjectTracker will be added here
	int Teardown();

	void SetCamera(Camera& camera);

private:

	static Renderer* renderer;

	GLFWwindow* window;
	int windowWidth, windowHeight;
	glm::vec4 backgroundColour;

	Camera camera;

	GLFWwindow* SetupGLFW();

	void SetWindow(int height, int width);
};