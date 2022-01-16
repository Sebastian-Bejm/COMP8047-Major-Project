#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld() {
	// Gravity is set to 0 because it will not be used here
	world = new b2World(b2Vec2(0.0f, 0.0f));
	world->SetAllowSleeping(false);

	contactListener = new ContactListener();
	world->SetContactListener(contactListener);
}

PhysicsWorld::~PhysicsWorld() {
	if (world) delete world;
	if (contactListener) delete contactListener;
}

PhysicsWorld& PhysicsWorld::GetInstance() {
	static PhysicsWorld physicsWorld;
	return physicsWorld;
}

// Adds a GameObject to the physics world by creating a new physics body
void PhysicsWorld::AddObject(GameObject* gameObject) {
	
	Transform* transform = gameObject->GetTransform();
	RigidBody* rigidBody = gameObject->GetRigidBody();

	b2BodyDef bodyDef = b2BodyDef();
	bodyDef.type = rigidBody->bodyType;
	bodyDef.position.Set(rigidBody->x, rigidBody->y);
	//bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(gameObject);

	b2World* world = PhysicsWorld::GetInstance().world;
	rigidBody->box2dBody = world->CreateBody(&bodyDef);

	if (rigidBody->box2dBody) {
		// The dynamic moving body will remain a 1x1 cube but use a circle fixture
		// This fixes the ghost collisions for now
		if (rigidBody->bodyType == b2_dynamicBody) {
			b2CircleShape bodyShape;
			bodyShape.m_radius = rigidBody->halfWidth;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &bodyShape;
			fixtureDef.density = rigidBody->density;
			fixtureDef.friction = rigidBody->friction;
			fixtureDef.restitution = 0;

			rigidBody->box2dBody->CreateFixture(&fixtureDef);
			//gameObject->SetRigidBody(rigidBody);
		}
		// For all other bodies they are made with box polygon shape
		else if (rigidBody->bodyType == b2_staticBody) {
			b2PolygonShape staticBox;
			staticBox.SetAsBox(rigidBody->halfWidth, rigidBody->halfHeight);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &staticBox;
			fixtureDef.density = rigidBody->density;
			fixtureDef.friction = rigidBody->friction;
			fixtureDef.restitution = 0;

			rigidBody->box2dBody->CreateFixture(&fixtureDef);
			//gameObject->SetRigidBody(rigidBody);
		}

	}
}

// Updates the physics bodies of all objects currently in the scene
void PhysicsWorld::Update(ObjectTracker* tracker) {
	if (world) {
		world->Step(timeStep, velocityIterations, positionIterations);

		std::vector<GameObject> objects = tracker->GetAllObjects();
		//std::cout << objects[0].GetRigidBody()->box2dBody->GetPosition().x 
			//<< " " << objects[0].GetRigidBody()->box2dBody->GetPosition().y << std::endl;

		for (int i = 0; i < objects.size(); i++) {
			RigidBody* rigidBody = objects[i].GetRigidBody();
			Transform* transform = objects[i].GetTransform();
			UpdateTransform(transform, rigidBody);
		}
	}
}

// Updates the transform of the GameObject based on the rigidbody position and state
void PhysicsWorld::UpdateTransform(Transform* transform, RigidBody* rigidBody) {
	float x = rigidBody->box2dBody->GetPosition().x;
	float y = rigidBody->box2dBody->GetPosition().y;
	transform->SetPosition(glm::vec3(x, y, transform->GetPosition().z));
}