#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLMHeader.h"
#include "Shader.h"

class Camera {
public:
	Camera();
	Camera(int viewWidth, int viewHeight, glm::vec3 position);

	void SetPosition(glm::vec3 newPosition);
	void SetOrthoMatrix(float left, float right, float bottom, float top, float nearPlane, float farPlane);
	void SetPerspectiveMatrix(float fovDeg, float nearPlane, float farPlane);
	void ProcessInput(GLFWwindow *window, float deltaTime);

	glm::mat4 GetCameraMatrix();
	glm::vec3 GetPosition();

private:

	float cameraSpeed = 0.005f;
	float sensitivity = 100.0f;

	bool isPerspective = false;
	bool firstClick = true;

	int viewWidth;
	int viewHeight;

	glm::vec3 position;
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 cameraMatrix;
};
