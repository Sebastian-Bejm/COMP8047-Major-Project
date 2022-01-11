#include "Mesh.h"

Mesh::Mesh() {}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	vao.Bind();
	// Generates Vertex Buffer object and links it to vertices
	vbo = VBO(vertices);
	// Generates Element Buffer object and links it to indices
	ebo = EBO(indices);

	// Links the attributes to the shader based on the layout
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // vertex
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float))); // color
	vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float))); // texture

	// Unbind to prevent modification
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();

	// Add texture to the mesh...

}

void Mesh::Draw(Shader& shader, Camera& camera) {
	// Activate the shader program for this mesh
	shader.Activate();
	vao.Bind();

	// Handle the texture
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++) {
		std::string num;
		std::string type = textures[i].GetType();

		// Using diffuse and specular in case I use lighting later
		if (type == "diffuse") {
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular") {
			num = std::to_string(numSpecular++);
		}
		textures[i].TexUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}
	
	glm::mat4 cam = camera.GetCameraMatrix();
	GLint cameraLoc = glGetUniformLocation(shader.GetID(), "camMatrix");
	glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(cam));

	// Draw the actual Mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::Delete() {
	vao.Delete();
	vbo.Delete();
	ebo.Delete();
	//glDeleteTextures(1, &texture);
}

