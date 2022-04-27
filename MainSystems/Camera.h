#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Utility/GLMHeader.h"
#include "Rendering/Shader.h"

class Camera {
public:
	Camera();
	Camera(int viewWidth, int viewHeight, glm::vec3 position);

	void SetPosition(glm::vec3 newPosition);
	void SetOrthoMatrix(float left, float right, float bottom, float top);
	void ProcessInput(GLFWwindow *window, float deltaTime, float& left, float& right, float& bottom, float& top);

	glm::mat4 GetCameraMatrix(); 
	glm::mat4 GetProjectionMatrix();
	glm::vec3 GetPosition();

private:

	int viewWidth;
	int viewHeight;

	float nearPlane = 0.1f;
	float farPlane = 100.0f;

	glm::vec3 position;
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 cameraMatrix;
};
