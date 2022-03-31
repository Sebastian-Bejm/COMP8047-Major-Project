#include "ObjectTracker.h"

// Returns the singleton instance of this ObjectTracker
ObjectTracker& ObjectTracker::GetInstance() {
	static ObjectTracker trackerInstance;
	return trackerInstance;
}

// Adds the GameObject to the tracker
void ObjectTracker::AddObject(GameObject& gameObject) {
	staticObjects.push_back(gameObject);
}

// Deletes all the memory used for the GameObjects
void ObjectTracker::DeleteAllObjects() {
	for (size_t i = 0; i < staticObjects.size(); i++) {
		staticObjects[i].Delete();
	}
}

// Returns a GameObject by its tag
GameObject& ObjectTracker::GetObjectByTag(std::string objectTag) {
	for (size_t i = 0; i < staticObjects.size(); i++) {
		if (staticObjects[i].GetTag() == objectTag) {
			return staticObjects[i];
		}
	}
}

// Returns all the objects in this tracker
std::vector<GameObject>& ObjectTracker::GetAllObjects() {
	return staticObjects;
}