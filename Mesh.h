#pragma once

#include <string>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
// include textures later

class Mesh {
public:

	Mesh();
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

	void Draw(Shader& shader, Camera& camera);
	void Delete();

private:

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	// vector for textures / normals as well

	VAO vao;
	
};
