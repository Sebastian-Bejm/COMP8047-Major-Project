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
	viewMatrix = glm::lookAt(position, position + orientation, up);
	projectionMatrix = glm::perspective(glm::radians(fovDeg), (float)(viewWidth / viewHeight), nearPlane, farPlane);
}

void Camera::MoveCamera(float movX, float movY, float movZ) {
	position.x += movX;
	position.y += movY;
	position.z += movZ;
}

void Camera::ProcessInput(GLFWwindow* window) {

}

glm::mat4 Camera::GetViewMatrix() {
	//glm::mat4 viewMat = glm::mat4(1.0f);
	//viewMatrix = glm::translate(viewMat, -position);
	return viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() {
	return projectionMatrix;
}
