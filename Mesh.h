#pragma once

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class Mesh {
public:

	Mesh();
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);

	void Draw(Shader& shader, Camera& camera);
	void Delete();

private:

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	VAO vao;
	VBO vbo;
	EBO ebo;
};
