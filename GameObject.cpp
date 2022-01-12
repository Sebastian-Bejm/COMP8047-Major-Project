#include "GameObject.h"

// Constructor to create a new GameObject
// A GameObject requires a tag, shape, shader, and transform
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
	Texture texture(textureFile.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

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
// Uses the camera's view and projection matrices to update the object's positions accordingly
void GameObject::Draw(Camera& camera) {
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

bool GameObject::CheckTexFileExtension(const std::string& textureFile, std::string ext) {
	// Handles without path
	std::string::size_type idx;
	idx = textureFile.rfind(".");

	if (idx != std::string::npos) {
		std::string extension = textureFile.substr(idx + 1);
		if (extension == ext) return true;
	}
	else {
		// no extension found
		return false;
	}
}
