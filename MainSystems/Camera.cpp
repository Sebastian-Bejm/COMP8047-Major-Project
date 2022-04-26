#include "Camera.h"

Camera::Camera() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	viewWidth = 1;
	viewHeight = 1;

	projectionMatrix = glm::mat4(1.0f);
	viewMatrix = glm::mat4(1.0f);
	cameraMatrix = glm::mat4(1.0f);
}

// Camera constructor: create a new camera that takes in our view width/height, and its new position
Camera::Camera(int viewWidth, int viewHeight, glm::vec3 position) {
	this->position = position;

	this->viewWidth = viewWidth;
	this->viewHeight = viewHeight;

	projectionMatrix = glm::mat4(1.0f);
	viewMatrix = glm::mat4(1.0f);
	cameraMatrix = glm::mat4(1.0f);
}

void Camera::SetPosition(glm::vec3 newPosition) {
	this->position = newPosition;
}

// Set the view and orthographic projection matrices
void Camera::SetOrthoMatrix(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
	viewMatrix = glm::lookAt(position, position + front, up);
	projectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);

	cameraMatrix = projectionMatrix * viewMatrix;
}

// Processes key and mouse inputs
// This is mainly used to look around our scene to ensure everything looks correct
void Camera::ProcessInput(GLFWwindow* window, float deltaTime) {
	float speed = 2.0f * deltaTime;

	// WASD moving
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position += speed * -glm::normalize(glm::cross(front, up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += speed * glm::normalize(glm::cross(front, up));
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += speed * up;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position += speed * -up;
	}

}

// Get our camera matrix (projection * view matrices)
glm::mat4 Camera::GetCameraMatrix() {
	return cameraMatrix;
}

// Get just the projection matrix from this camera
glm::mat4 Camera::GetProjectionMatrix() {
	return projectionMatrix;
}

// Get the position of our camera
glm::vec3 Camera::GetPosition() {
	return position;
}

