#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Camera.h"
#include "ObjectTracker.h"

extern GLFWwindow* window;

class Renderer {
public:

	static Renderer* GetInstance();

	int Init(glm::vec4 backgroundColor, int windowWidth, int windowHeight);
	int Update(ObjectTracker* tracker); // ObjectTracker will be added here
	int Teardown();

	void SetCamera(Camera& camera);

private:

	static Renderer* renderer;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	VAO vao;
	Texture crateTexture;
	Texture brickTexture;

	float deltaTime;
	float currentFrame, lastFrame;

	int windowWidth, windowHeight;
	glm::vec4 backgroundColour;

	Camera camera;

	void PrepareGLBuffers();
	void CreateTextureBuffers();
	GLFWwindow* SetupGLFW();
	void SetWindow(int width, int height);
};