#pragma once

#include <box2d/box2d.h>

struct RigidBody {

	b2BodyType bodyType;

	float halfWidth, halfHeight;
	float x, y;

	float density;
	float friction;

	b2Body* box2dBody;
};