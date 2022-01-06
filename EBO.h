#pragma once

#include <glad/glad.h>

class EBO {
public:

	EBO(GLuint* indices, GLsizeiptr size) {
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	void Bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	}

	void Unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Delete() {
		glDeleteBuffers(1, &ID);
	}

private:

	GLuint ID;

};