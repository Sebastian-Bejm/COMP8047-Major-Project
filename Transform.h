#pragma once

#include "GLMHeader.h"

class Transform
{
public:
	
	Transform();
	Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	void Translate(glm::vec3 translate, float speed);
	void Rotate(glm::vec3 rotate);

	glm::mat4 GetModelMatrix();

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();

private:

	glm::mat4 modelMatrix;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

};

