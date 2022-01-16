#pragma once

#include <iostream>

#include "GameObject.h"
#include "ObjectTracker.h"
#include "ContactListener.h"

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	static PhysicsWorld& GetInstance();
	void AddObject(GameObject* gameObject);
	void Update(ObjectTracker* tracker);
	void CreateChainLoop(b2Vec2* vertices);

private:
	const float timeStep = 1.0f / 60.0f;
	const int velocityIterations = 8; // 6
	const int positionIterations = 3; // 2

	b2World* world;
	ContactListener* contactListener;

	void UpdateTransform(Transform* transform, RigidBody* rigidBody);
};
