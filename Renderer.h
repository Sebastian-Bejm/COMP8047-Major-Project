#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GLMHeader.h"

class Renderer {
public:

	int Init(int viewWidth, int viewHeight, glm::vec4 backgroundColor);
	int Update(); // ObjectTracker will be added here
	int Teardown();


private:

	static Renderer* renderer;
	int windowWidth, windowHeight;

	glm::vec4 backgroundColor;

	void SetWindow(int height, int width);
	void DrawObjects();
};