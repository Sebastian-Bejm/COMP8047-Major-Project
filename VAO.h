#pragma once

#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:

	VAO() {
		glGenVertexArrays(1, &ID);
	}

	void LinkVBO(VBO vbo, GLuint layout) {
		vbo.Bind();
		glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
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