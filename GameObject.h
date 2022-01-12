#pragma once

#include <filesystem>

#include "Mesh.h"
#include "Transform.h"
#include "ShapeDetails.h"

class GameObject
{
public:

	GameObject(std::string tag, std::string textureFile, ShapeType shapeType, Shader& shader, 
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	
	Transform& GetTransform();

	void Draw(Camera& camera);
	void Delete();

private:
	std::string objectTag;

	Shader shaderProgram;
	Mesh mesh;
	Transform transform;

	bool CheckTexFileExtension(const std::string& textureFile, std::string ext); // temp
};

