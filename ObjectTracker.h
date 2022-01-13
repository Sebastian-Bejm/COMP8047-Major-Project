#pragma once

#include <vector>

#include "GameObject.h"

class ObjectTracker
{
public:

	ObjectTracker* GetInstance();

	void Add(GameObject* gameObject);
	GameObject* FindByTag(std::string objectTag);
	void DeleteAllObjects();

private:

	static ObjectTracker* instance;

	std::vector<GameObject*> staticObjects;

};

