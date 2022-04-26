#pragma once

#include "Rendering/Shader.h"
#include "MainSystems/Camera.h"
#include "Transform.h"
#include "RigidBody.h"

class GameObject
{
public:

	GameObject(std::string tag, std::string textureFile, Shader& shader, 
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	
	Transform* GetTransform();
	void ResetTransform();

	void SetPosition(float posX, float posY);
	void SetVelocity(float velX, float velY);

	void SetRigidBody(RigidBody* rigidBody);
	void SetBodyType(b2BodyType type);
	RigidBody* GetRigidBody();

	Shader& GetShader();
	std::string GetTag();
	int GetTextureID();

	void Draw(Camera& camera);
	void Delete();

private:

	std::string objectTag;
	std::string imageFile;

	Shader shaderProgram;

	Transform* transform;
	RigidBody* rigidBody;

};

