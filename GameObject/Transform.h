#pragma once

#include <iostream>
#include <tuple>

#include "Utility/GLMHeader.h"

class Transform
{
public:
	
	Transform();
	Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	void SetPosition(glm::vec3 newPosition);
	void Translate(glm::vec3 translate, float speed);

	glm::mat4 GetModelMatrix();
	glm::vec3 GetOriginalPosition();

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();

private:

	glm::mat4 modelMatrix;

	glm::vec3 position, rotation, scale;
	glm::vec3 originalPos, originalRotation, originalScale;

};

