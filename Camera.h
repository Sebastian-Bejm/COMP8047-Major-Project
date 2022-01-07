#pragma once

#include <glad//glad.h>
#include <GLFW/glfw3.h>
#include "GLMHeader.h"

class Camera {
public:
	Camera();
	Camera(int viewWidth, int viewHeight, glm::vec3 position);
	~Camera();

	void SetMatrix(float fovDeg, float nearPlane, float farPlane);
	void MoveCamera(float movX, float movY, float movZ);
	void ProcessInput(GLFWwindow *window);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

private:

	float speed = 0.1f;
	float sensitivity = 100.0f;

	int viewWidth;
	int viewHeight;

	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
};
