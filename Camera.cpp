#include "Camera.h"

Camera::Camera() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	viewWidth = 1;
	viewHeight = 1;
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

// Set our view and projection matrices
// 
void Camera::SetMatrix(float fovDeg, float nearPlane, float farPlane) {

	viewMatrix = glm::lookAt(position, position + front, up);
	projectionMatrix = glm::perspective(glm::radians(fovDeg), (float)(viewWidth / viewHeight), nearPlane, farPlane);

	cameraMatrix = projectionMatrix * viewMatrix;
}

// Processes key and mouse inputs
// This is mainly used to look around our scene to ensure everything looks correct
void Camera::ProcessInput(GLFWwindow* window, float deltaTime) {

	float speed = 0.5f * deltaTime;

	// Key inputs
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

	// Mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick) {
			glfwSetCursorPos(window, (viewWidth / 2), (viewHeight / 2));
			firstClick = false;
		}

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (viewHeight / 2)) / viewHeight;
		float rotY = sensitivity * (float)(mouseX - (viewWidth / 2)) / viewWidth;

		glm::vec3 newOrientation = glm::rotate(front, glm::radians(-rotX), glm::normalize(glm::cross(front, up)));

		if (!(glm::angle(newOrientation, up) <= glm::radians(5.0f) || glm::angle(newOrientation, -up) <= glm::radians(5.0f))) {
			front = newOrientation;
		}

		front = glm::rotate(front, glm::radians(-rotY), up);

		glfwSetCursorPos(window, (viewWidth / 2), (viewHeight / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}

// Get our camera matrix (projection * view matrices)
glm::mat4 Camera::GetCameraMatrix() {
	return cameraMatrix;
}

// Get the position of our camera
glm::vec3 Camera::GetPosition() {
	return position;
}

