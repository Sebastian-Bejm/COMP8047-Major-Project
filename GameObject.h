#pragma once

#include "Mesh.h"
#include "Transform.h"
#include "RigidBody.h"
#include "ShapeDetails.h"

class GameObject
{
public:

	GameObject(std::string tag, std::string textureFile, ShapeType shapeType, Shader& shader, 
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	
	Transform* GetTransform();

	void SetRigidBody(RigidBody* rigidBody);
	void SetBodyType(b2BodyType type);
	RigidBody* GetRigidBody();

	std::string GetTag();

	void Draw(Camera& camera);
	void Delete();

private:
	std::string objectTag;

	Shader shaderProgram;
	Mesh mesh;
	Transform* transform;
	RigidBody* rigidBody;

	std::string GetTextureFileExtension(const std::string& textureFile);
};

