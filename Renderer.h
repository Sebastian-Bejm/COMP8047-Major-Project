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

	//VAO vao;
	//VBO vbo;
	//EBO ebo;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	float deltaTime;
	float currentFrame, lastFrame;

	int windowWidth, windowHeight;
	glm::vec4 backgroundColour;

	Camera camera;

	void PrepareGLBuffers();
	GLFWwindow* SetupGLFW();
	void SetWindow(int width, int height);
};