#pragma once

#include <string>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"
// include textures later

class Mesh {
public:

	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Shader& shader);

private:

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	// vector for textures as well

	VAO vao;
	
};
