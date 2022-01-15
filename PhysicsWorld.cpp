#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld() {
	// Gravity is set to 0 because it will not be used here
	world = new b2World(b2Vec2(0.1f, -0.1f));
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

void PhysicsWorld::AddObject(GameObject* gameObject) {
	
	Transform* transform = gameObject->GetTransform();
	RigidBody* rigidBody = gameObject->GetRigidBody();

	b2BodyDef bodyDef = b2BodyDef();
	bodyDef.type = rigidBody->bodyType;
	bodyDef.position.Set(rigidBody->x, rigidBody->y);
	//bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(gameObject);

	b2World* world = PhysicsWorld::GetInstance().world;
	rigidBody->box2dBody = world->CreateBody(&bodyDef);
	rigidBody->box2dBody->SetFixedRotation(true);

	if (rigidBody->box2dBody) {
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(rigidBody->halfWidth, rigidBody->halfHeight);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = rigidBody->density;
		fixtureDef.friction = rigidBody->friction;
		fixtureDef.restitution = 0;

		rigidBody->box2dBody->CreateFixture(&fixtureDef);
		gameObject->SetRigidBody(rigidBody);
	}
}

void PhysicsWorld::Update(ObjectTracker* tracker) {
	if (world) {
		world->Step(timeStep, velocityIterations, positionIterations);

		std::vector<GameObject> objects = tracker->GetAllObjects();

		std::cout << objects[0].GetRigidBody()->box2dBody->GetPosition().y << std::endl;

		for (int i = 0; i < objects.size(); i++) {
			RigidBody* rigidBody = objects[i].GetRigidBody();
			Transform* transform = objects[i].GetTransform();
			UpdateTransform(transform, rigidBody);
		}
	}
}

void PhysicsWorld::UpdateTransform(Transform* transform, RigidBody* rigidBody) {
	float x = rigidBody->box2dBody->GetPosition().x;
	float y = rigidBody->box2dBody->GetPosition().y;
	transform->SetPosition(glm::vec3(x, y, transform->GetPosition().z));
}