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

	// Handle texture drawing here later
	
	glm::mat4 cam = camera.GetCameraMatrix();

	GLint cameraLoc = glGetUniformLocation(shader.GetID(), "camMatrix");
	glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(cam));

	// Draw primitives, number of indices, datatype of indices, index of indices
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::Cleanup() {

}

