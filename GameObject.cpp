#include "GameObject.h"

GameObject::GameObject() {

}

GameObject::GameObject(std::string tag, Shader& shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

	objectTag = tag;
	shaderProgram = shader;

	std::vector<Vertex> vertices(cubeVertices, cubeVertices + sizeof(cubeVertices) / sizeof(Vertex));
	std::vector<GLuint> indices(cubeIndices, cubeIndices + sizeof(cubeIndices) / sizeof(GLuint));

	mesh = Mesh(vertices, indices);
	transform = Transform(position, rotation, scale);

	shaderProgram.Activate();
	GLint cubeLoc = glGetUniformLocation(shaderProgram.GetID(), "model");
	glUniformMatrix4fv(cubeLoc, 1, GL_FALSE, glm::value_ptr(transform.GetModelMatrix()));
}

Transform GameObject::GetTransform() {
	return transform;
}

void GameObject::Update(Camera& camera) {
	// Update the object model matrix first

	GLint cubeLoc = glGetUniformLocation(shaderProgram.GetID(), "model");
	glUniformMatrix4fv(cubeLoc, 1, GL_FALSE, glm::value_ptr(transform.GetModelMatrix()));

	// Draw the mesh afterwards
	mesh.Draw(shaderProgram, camera);
}

void GameObject::Delete() {
	shaderProgram.Delete();
}