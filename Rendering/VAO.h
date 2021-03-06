#pragma once

#include "VBO.h"

class VAO {
public:

	VAO() {}

	void InitVAO() {
		glGenVertexArrays(1, &ID);
	}

	void LinkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
		vbo.Bind();
		glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);
		vbo.Unbind();
	}

	void Bind() {
		glBindVertexArray(ID);
	}

	void Unbind() {
		glBindVertexArray(0);
	}

	void Delete() {
		glDeleteVertexArrays(1, &ID);
	}

private:

	GLuint ID;

};