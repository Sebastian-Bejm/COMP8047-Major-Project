#pragma once

#include <vector>

#include "GameObject.h"

class ObjectTracker
{
public:

	static ObjectTracker* GetInstance();

	void Add(GameObject& gameObject);
	void DeleteAllObjects();

	std::vector<GameObject>& GetAllObjects();

	GameObject& FindByTag(std::string objectTag);


private:

	static ObjectTracker* instance;

	std::vector<GameObject> staticObjects;

};
