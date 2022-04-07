#include "GameObject.h"

// Constructor to create a new GameObject
// A GameObject requires a tag, texture file, shape, shader, and transform
GameObject::GameObject(std::string tag, std::string textureFile, Shader& shader,
	glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

	objectTag = tag;
	imageFile = textureFile;
	shaderProgram = shader;

	// Set the initial transform to this object
	transform = new Transform(position, rotation, scale);

	// Set initial rigidbody in game object
	rigidBody = new RigidBody();
	rigidBody->bodyType = b2_staticBody; // Each body is static by default
	rigidBody->density = 1;
	rigidBody->friction = 0;

	rigidBody->x = transform->GetPosition().x;
	rigidBody->y = transform->GetPosition().y;
	rigidBody->halfWidth = transform->GetScale().x / 2;
	rigidBody->halfHeight = transform->GetScale().y / 2;
}

// Get the transform of this object
// Used to help manipulate the position of object when necessary
Transform* GameObject::GetTransform() {
	return transform;
}

// Reset the rigidbody to the original position of the transform
void GameObject::ResetTransform() {
	glm::vec3 originalPos = transform->GetOriginalPosition();

	rigidBody->box2dBody->SetTransform(b2Vec2(originalPos.x, originalPos.y), rigidBody->box2dBody->GetAngle());

	transform->SetPosition(glm::vec3(rigidBody->box2dBody->GetPosition().x, 
		rigidBody->box2dBody->GetPosition().y, transform->GetPosition().z));
}

// Set this object's position to
void GameObject::SetPosition(float posX, float posY) {
	rigidBody->box2dBody->SetTransform(b2Vec2(posX, posY), rigidBody->box2dBody->GetAngle());

	transform->SetPosition(glm::vec3(rigidBody->box2dBody->GetPosition().x,
		rigidBody->box2dBody->GetPosition().y, transform->GetPosition().z));
}	

// Set the linear velocity for this object's rigidbody
void GameObject::SetVelocity(float velX, float velY) {
	rigidBody->box2dBody->SetLinearVelocity(b2Vec2(velX, velY));
}

// Set the rigidbody for this object
void GameObject::SetRigidBody(RigidBody* rigidBody) {
	this->rigidBody = rigidBody;
}

// Set the rigidbody body type for this object
void GameObject::SetBodyType(b2BodyType type) {
	rigidBody->bodyType = type;
}

// Get the rigidbody of this object
// Used to help manipulate the physics attached to this object
RigidBody* GameObject::GetRigidBody() {
	return rigidBody;
}

Shader& GameObject::GetShader() {
	return shaderProgram;
}

// Get the tag of this game object
std::string GameObject::GetTag() {
	return objectTag;
}

// Get the ID of the texture image passed into the object
int GameObject::GetTextureID() {
	if (imageFile == "crate.jpg") {
		return 0;
	}
	else if (imageFile == "brick.png") {
		return 1;
	}
	else if (imageFile == "start_tex.jpg") {
		return 2;
	}
	else if (imageFile == "end_tex.jpg") {
		return 3;
	}
	else if (imageFile == "lava.png") {
		return 4;
	}
	return -1;
}

// Update this GameObject's matrices. 
// Uses the camera's view and projection matrices to update the object's positions accordingly
void GameObject::Draw(Camera& camera) {

	// Use the camera matrix to tell OpenGL where to look at in the world space
	GLint cameraLoc = glGetUniformLocation(shaderProgram.GetID(), "camMatrix");
	glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(camera.GetCameraMatrix()));

	// Update the object model to show the objects actual position in world space
	GLint cubeLoc = glGetUniformLocation(shaderProgram.GetID(), "model");
	glUniformMatrix4fv(cubeLoc, 1, GL_FALSE, glm::value_ptr(transform->GetModelMatrix()));
}

// Delete the contents of this GameObject
void GameObject::Delete() {
	// Delete rigidbody and transform 
	delete rigidBody;
	delete transform;
}
