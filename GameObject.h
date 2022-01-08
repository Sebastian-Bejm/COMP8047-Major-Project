#pragma once

#include "Mesh.h"
#include "Transform.h"

class GameObject
{
public:

	GameObject();
	GameObject(std::string name, glm::vec3 position);
	
	void Update();
	void Delete();

private:

	Mesh mesh;

	std::string name;

	// vertices and indices info will be in this class later on

};

