#include "GameObject.h"

// Constructor to create a new GameObject
// A GameObject requires a tag, shape, shader, and transform
GameObject::GameObject(std::string tag, std::string textureName, ShapeType shapeType, Shader& shader,
	glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

	objectTag = tag;
	shaderProgram = shader;

	// Retrieve vertices/indices data for a shape
	ShapeDetails shapeDetails;
	Shape shape = shapeDetails.GetShape(shapeType);
	
	std::vector<Vertex> vertices = shape.vertices;
	std::vector<GLuint> indices = shape.indices;

	// Initialize the texture for this object
	Texture texture(textureName.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	// Create a mesh with the vertices, indices, and transform
	mesh = Mesh(vertices, indices, texture);
	transform = Transform(position, rotation, scale);

	// Set the model matrix 
	GLint cubeLoc = glGetUniformLocation(shaderProgram.GetID(), "model");
	glUniformMatrix4fv(cubeLoc, 1, GL_FALSE, glm::value_ptr(transform.GetModelMatrix()));
}

Transform& GameObject::GetTransform() {
	return transform;
}

// Update this GameObject's matrices. 
// Uses the camera's view and projection matrices to update everything accordingly
void GameObject::Update(Camera& camera) {
	// Update the object model matrix first
	GLint cubeLoc = glGetUniformLocation(shaderProgram.GetID(), "model");
	glUniformMatrix4fv(cubeLoc, 1, GL_FALSE, glm::value_ptr(transform.GetModelMatrix()));

	// Draw the mesh afterwards (updates the projection and view matrices from the camera)
	mesh.Draw(shaderProgram, camera);
}

// Delete the contents of this GameObject
void GameObject::Delete() {
	mesh.Delete();
	shaderProgram.Delete();
}