#pragma once

#include <iostream>

#include "GameObject/GameObject.h"
#include "MainSystems/ObjectTracker.h"
#include "ContactListener.h"

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	static PhysicsWorld& GetInstance();
	void AddObject(GameObject* gameObject);
	void Update(ObjectTracker* tracker);
	void DestroyObjects();

private:
	// A timestep in the physics engine is 1/60
	const float timeStep = 1.0f / 60.0f; 
	const int velocityIterations = 6; 
	const int positionIterations = 2;

	b2World* world;
	ContactListener* contactListener;

	void UpdateTransform(Transform* transform, RigidBody* rigidBody);
};

