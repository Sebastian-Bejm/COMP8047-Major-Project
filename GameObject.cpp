#include "GameObject.h"

GameObject::GameObject(std::string tag, Shader& shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

	objectTag = tag;
	shaderProgram = shader;

	// Cube
	Vertex cubeVerts[] = {
		//		Coordinates				 ,		Colors
		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
		glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
		glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
		glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
		glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
		glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
		glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
		glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
		glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
		glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
		glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
		glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
		glm::vec3(-0.5f,  0.5f, 0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
		glm::vec3(0.5f,  0.5f, 0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
		glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
		glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
		glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
		glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
		glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
		glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
		glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
		glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
	};

	GLint cubeInds[] = {
		0, 2, 1,
		0, 3, 2,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 15, 14,
		12, 14, 13,
		16, 17, 18,
		16, 18, 19,
		20, 23, 22,
		20, 22, 21
	};

	std::vector<Vertex> vertices(cubeVerts, cubeVerts + sizeof(cubeVerts) / sizeof(Vertex));
	std::vector<GLuint> indices(cubeInds, cubeInds + sizeof(cubeInds) / sizeof(GLuint));

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