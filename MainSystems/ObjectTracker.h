#pragma once

#include <vector>

#include "GameObject/GameObject.h"

class ObjectTracker
{
public:

	static ObjectTracker& GetInstance();

	void AddObject(GameObject& gameObject);
	void RemoveAllObjects();
	void DeleteAllObjects();

	GameObject& GetObjectByTag(std::string objectTag);
	std::vector<GameObject>& GetAllObjects();

private:

	std::vector<GameObject> objectStorage;

};