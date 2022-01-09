#pragma once

#include "Mesh.h"
#include "Transform.h"

class GameObject
{
public:

	GameObject(std::string tag, Shader& shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	
	Transform GetTransform();

	void Update(Camera& camera);
	void Delete();

private:
	std::string objectTag;

	Shader shaderProgram;
	Mesh mesh;
	Transform transform;
};

