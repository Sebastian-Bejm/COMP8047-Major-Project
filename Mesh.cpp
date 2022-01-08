#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices) {
	this->vertices = vertices;
	this->indices = indices;

	// Create the VAO, VBO, and EBO
	vao.Bind();

	VBO vbo(vertices);
	EBO ebo(indices);

	// Links the attributes to the shader based on layout
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // vertex
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3*sizeof(float))); // color

	// Unbind to prevent modification
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera) {
	// Activate the shader program
	shader.Activate();
	// Bind the VAO so OpenGL knows to use it
	vao.Bind();

	// Handle texture drawing here later
	
	glm::mat4 cam = camera.GetCameraMatrix();

	GLint cameraLoc = glGetUniformLocation(shader.GetID(), "camMatrix");
	glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(cam));

	// Draw the actual Mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::Cleanup() {

}

