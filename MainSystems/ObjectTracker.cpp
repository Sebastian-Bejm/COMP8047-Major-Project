#include "ObjectTracker.h"

// Returns the singleton instance of this ObjectTracker
ObjectTracker& ObjectTracker::GetInstance() {
	static ObjectTracker trackerInstance;
	return trackerInstance;
}

// Adds the GameObject to the tracker
void ObjectTracker::AddObject(GameObject& gameObject) {
	objectStorage.push_back(gameObject);
}

// Removes the objects, but does not delete the memory
void ObjectTracker::RemoveAllObjects() {
	objectStorage.clear();
}


// Deletes all the memory used for the GameObjects
void ObjectTracker::DeleteAllObjects() {
	for (size_t i = 0; i < objectStorage.size(); i++) {
		objectStorage[i].Delete();
	}
}

// Returns a GameObject by its tag
GameObject& ObjectTracker::GetObjectByTag(std::string objectTag) {
	for (size_t i = 0; i < objectStorage.size(); i++) {
		if (objectStorage[i].GetTag() == objectTag) {
			return objectStorage[i];
		}
	}
}

// Returns all the objects in this tracker
std::vector<GameObject>& ObjectTracker::GetAllObjects() {
	return objectStorage;
}