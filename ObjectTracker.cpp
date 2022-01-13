#include "ObjectTracker.h"

ObjectTracker* ObjectTracker::instance = nullptr;

ObjectTracker* ObjectTracker::GetInstance() {
	if (instance == nullptr) {
		instance = new ObjectTracker();
	}
	return instance;
}

void ObjectTracker::Add(GameObject& gameObject) {
	staticObjects.push_back(gameObject);
}

void ObjectTracker::DeleteAllObjects() {
	for (int i = 0; i < staticObjects.size(); i++) {
		staticObjects[i].Delete();
	}
}

std::vector<GameObject>& ObjectTracker::GetAllObjects() {
	return staticObjects;
}

GameObject& ObjectTracker::FindByTag(std::string objectTag) {
	for (int i = 0; i < staticObjects.size(); i++) {
		if (staticObjects[i].GetTag() == objectTag) {
			return staticObjects[i];
		}
	}
}