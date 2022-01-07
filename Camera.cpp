#include "Camera.h"

Camera::Camera() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	viewWidth = 1;
	viewHeight = 1;
	projectionMatrix = glm::mat4(1.0f);
	viewMatrix = glm::mat4(1.0f);
}

Camera::Camera(int viewWidth, int viewHeight, glm::vec3 position) {
	this->position = position;

	this->viewWidth = viewWidth;
	this->viewHeight = viewHeight;
	projectionMatrix = glm::mat4(1.0f);
	viewMatrix = glm::mat4(1.0f);
}

Camera::~Camera() {

}

// Currently made for perspective, change to orthographic later on
void Camera::SetMatrix(float fovDeg, float nearPlane, float farPlane) {
	viewMatrix = glm::lookAt(position, position + front, up);
	projectionMatrix = glm::perspective(glm::radians(fovDeg), (float)(viewWidth / viewHeight), nearPlane, farPlane);
}

/*
* Processes inputs to move the camera. This will primarily be used for debugging the scene.
*/
void Camera::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += speed * front;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position += speed * -glm::normalize(glm::cross(front, up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position += speed * -front;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += speed * glm::normalize(glm::cross(front, up));
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += speed * up;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position += speed * -up;
	}
}

glm::mat4 Camera::GetViewMatrix() {
	return viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() {
	return projectionMatrix;
}
