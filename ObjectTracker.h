#pragma once

#include <vector>

#include "GameObject.h"

class ObjectTracker
{
public:

	static ObjectTracker& GetInstance();

	void AddObject(GameObject& gameObject);
	void DeleteAllObjects();

	GameObject& GetObjectByTag(std::string objectTag);
	std::vector<std::reference_wrapper<GameObject>> GetObjectsByTag(std::string objectTag);
	std::vector<GameObject>& GetAllObjects();

private:

	std::vector<GameObject> objectStorage;

};