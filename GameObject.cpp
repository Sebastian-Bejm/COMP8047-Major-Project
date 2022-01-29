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

int GameObject::GetTextureID() {
	int texID;
	if (imageFile == "crate.jpg") {
		texID = 0;
	}
	else if (imageFile == "brick.png") {
		texID = 1;
	}
	else {
		texID = -1;
	}
	return texID;
}

// Update this GameObject's matrices. 
// Uses the camera's view and projection matrices to update the object's positions accordingly
void GameObject::Draw(Camera& camera) {

	//texture.TexUnit(shaderProgram, "tex0", 0);
	//texture.Bind();

	// Update the object model after drawing initial object
	//GLint cubeLoc = glGetUniformLocation(shaderProgram.GetID(), "model");
	//glUniformMatrix4fv(cubeLoc, 1, GL_FALSE, glm::value_ptr(transform->GetModelMatrix()));
}

// Delete the contents of this GameObject
void GameObject::Delete() {
	// Delete rigidbody and transform 
	delete rigidBody;
	delete transform;
}
