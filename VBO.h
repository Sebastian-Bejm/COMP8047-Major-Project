#pragma once

#include <glad/glad.h>
#include <vector>

#include "Vertex.h"

class VBO {
public:

	VBO() {}

	VBO(std::vector<Vertex>& vertices) {
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	}

	void Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}

	void Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Delete() {
		glDeleteBuffers(1, &ID);
	}

private:

	GLuint ID;

};