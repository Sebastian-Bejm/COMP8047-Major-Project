#include "GameObject.h"

// Constructor to create a new GameObject
// A GameObject requires a tag, texture file, shape, shader, and transform
GameObject::GameObject(std::string tag, std::string textureFile, ShapeType shapeType, Shader& shader,
	glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

	objectTag = tag;
	shaderProgram = shader;

	// Retrieve vertices/indices data for a shape
	ShapeDetails shapeDetails;
	Shape shape = shapeDetails.GetShape(shapeType);
	
	std::vector<Vertex> vertices = shape.vertices;
	std::vector<GLuint> indices = shape.indices;

	// Initialize the texture for this object
	// GL_RGBA for png
	// GL_RGB for jpg
	GLenum format = NULL;
	std::string ext = GetTextureFileExtension(textureFile);

	if (ext == "png") {
		format = GL_RGBA;
	}
	else if (ext == "jpg") {
		format = GL_RGB;
	}
	Texture texture(textureFile.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, format, GL_UNSIGNED_BYTE);

	// Create a mesh with the vertices, indices, and transform
	mesh = Mesh(vertices, indices, texture);
	transform = new Transform(position, rotation, scale);

	// Set initial rigidbody
	rigidBody = new RigidBody();
	rigidBody->bodyType = b2_staticBody;
	rigidBody->density = 1;
	rigidBody->friction = 0;

	rigidBody->x = transform->GetPosition().x;
	rigidBody->y = transform->GetPosition().y;
	rigidBody->halfWidth = transform->GetScale().x / 2;
	rigidBody->halfHeight = transform->GetScale().y / 2;

	// Set the model matrix 
	GLint cubeLoc = glGetUniformLocation(shaderProgram.GetID(), "model");
	glUniformMatrix4fv(cubeLoc, 1, GL_FALSE, glm::value_ptr(transform->GetModelMatrix()));
}

// Get the transform of this object
// Used to help manipulate the position of object when necessary
Transform* GameObject::GetTransform() {
	return transform;
}

void GameObject::SetRigidBody(RigidBody* rigidBody) {
	this->rigidBody = rigidBody;
}

void GameObject::SetBodyType(b2BodyType type) {
	rigidBody->bodyType = type;
}

RigidBody* GameObject::GetRigidBody() {
	return rigidBody;
}

// Get the tag of this game object
std::string GameObject::GetTag() {
	return objectTag;
}

// Update this GameObject's matrices. 
// Uses the camera's view and projection matrices to update the object's positions accordingly
void GameObject::Draw(Camera& camera) {
	// Draw the mesh first to avoid wrong textures
	mesh.Draw(shaderProgram, camera);

	// Update the object model after drawing initial object
	GLint cubeLoc = glGetUniformLocation(shaderProgram.GetID(), "model");
	glUniformMatrix4fv(cubeLoc, 1, GL_FALSE, glm::value_ptr(transform->GetModelMatrix()));
}

// Delete the contents of this GameObject
void GameObject::Delete() {
	// Delete everything in the mesh
	mesh.Delete();

	// Delete rigidbody
	delete rigidBody;
	delete transform;

	//std::cout << "Shader ID: " << shaderProgram.GetID() << std::endl;
	shaderProgram.Delete();
}

std::string GameObject::GetTextureFileExtension(const std::string& textureFile) {
	size_t i = textureFile.rfind('.', textureFile.length());
	if (i != std::string::npos) {
		return (textureFile.substr(i + 1, textureFile.length() - i));
	}
	return "";
}

