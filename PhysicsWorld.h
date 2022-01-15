#pragma once

#include "RigidBody.h"
#include "ContactListener.h"
#include "GameObject.h"
#include "ObjectTracker.h"

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	void AddPhysics(GameObject* gameObject);
	void Update(ObjectTracker* tracker);

private:
	const float timeStep = 1.0f / 60.0f;
	const int velocityIterations = 6;
	const int positionIterations = 2;

	b2Vec2* gravity;
	b2World* world;
	ContactListener* contactListener;

	void UpdateTransform(Transform* transform, RigidBody* rigidBody);
};

