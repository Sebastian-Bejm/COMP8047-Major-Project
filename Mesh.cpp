#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices) {
	this->vertices = vertices;
	this->indices = indices;

	vao.Bind();

	VBO vbo(vertices);
	EBO ebo(indices);

	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // vertex
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3*sizeof(float))); // normal

	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera) {
	shader.Activate();
	vao.Bind();

	// texture draw here
	
	glUniform3f(glGetUniformLocation(shader.GetID(), "camPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
}

