#pragma once

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class Mesh {
public:

	Mesh();
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Texture& texture);

	void Draw(Shader& shader, Camera& camera);
	void Delete();

private:

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	
	Texture texture;
	VAO vao;

};
