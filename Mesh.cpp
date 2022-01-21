#include "Mesh.h"

Mesh::Mesh() {}

// Mesh constructor: create a new mesh using vertices and indicies of a shape, and using a new texture
Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Texture& texture) {
	this->vertices = vertices;
	this->indices = indices;
	this->texture = texture;

	vao.Bind();
	// Generates Vertex Buffer object and links it to vertices
	vbo = VBO(vertices);
	// Generates Element Buffer object and links it to indices
	ebo = EBO(indices);

	// Links the attributes to the shader based on the layout
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // coordinates
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float))); // color
	vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float))); // texture

	// Unbind to prevent modification
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}

// Draw the mesh of this object by using a shader and the camera of the scene
void Mesh::Draw(Shader& shader, Camera& camera) {
	// Bind shader in the Mesh to be able to access uniforms
	shader.Activate();
	vao.Bind();

	// Handle the texture
	texture.TexUnit(shader, "tex0", 0);
	texture.Bind();

	// Get our camera matrix in order to update the matrices to show where this new object is
	glm::mat4 cam = camera.GetCameraMatrix();
	GLint cameraLoc = glGetUniformLocation(shader.GetID(), "camMatrix");
	glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(cam));

	// Draw the actual Mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

// Cleanup our VAO, VBO, EBO and texture in this mesh
void Mesh::Delete() {
	vao.Delete();
	vbo.Delete();
	ebo.Delete();
	texture.Delete();
}

