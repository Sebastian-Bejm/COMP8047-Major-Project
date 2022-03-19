#pragma once

#include <vector>

#include "GameObject.h"

class ObjectTracker
{
public:

	static ObjectTracker& GetInstance();

	void AddObject(GameObject& gameObject);
	void DeleteAllObjects();

	std::vector<GameObject>& GetAllObjects();

	GameObject& FindByTag(std::string objectTag);

private:

	std::vector<GameObject> staticObjects;

};

