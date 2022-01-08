#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Shader& shader) {
	this->vertices = vertices;
	this->indices = indices;

	vao.Bind();

	VBO vbo(vertices);
	EBO ebo(indices);

	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);

}
