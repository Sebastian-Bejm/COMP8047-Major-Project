#include "Transform.h"

Transform::Transform() {
	modelMatrix = glm::mat4(1.0f);
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
}

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
	modelMatrix = glm::mat4(1.0f);

	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

void Transform::SetPosition(glm::vec3 newPosition) {
	position = newPosition;
}

void Transform::Translate(glm::vec3 translate, float speed) {
	position.x += translate.x * speed;
	position.y += translate.y * speed;
	position.z += translate.z * speed;
}

void Transform::Rotate(glm::vec3 rotate) {

}

glm::mat4 Transform::GetModelMatrix() {
	modelMatrix = glm::translate(position);
	// rotation later
	modelMatrix = glm::scale(modelMatrix, scale);
	return modelMatrix;
}

glm::vec3 Transform::GetPosition() {
	return position;
}

glm::vec3 Transform::GetRotation() {
	return rotation;
}

glm::vec3 Transform::GetScale() {
	return scale;
}