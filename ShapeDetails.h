#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "Vertex.h"

struct Shape {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
};

class ShapeDetails {
public:

	static Shape GetCube() {
		Vertex cubeVerts[] = {
			//		Coordinates				 ,		Colors
			glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
			glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
			glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
			glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
			glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
			glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
			glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
			glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
			glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
			glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
			glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
			glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
			glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
			glm::vec3(-0.5f,  0.5f, 0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
			glm::vec3(0.5f,  0.5f, 0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
			glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
			glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
			glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
			glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
			glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
			glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
			glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
			glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
			glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
		};

		GLint cubeInds[] = {
			0, 2, 1,
			0, 3, 2,
			4, 5, 6,
			4, 6, 7,
			8, 9, 10,
			8, 10, 11,
			12, 15, 14,
			12, 14, 13,
			16, 17, 18,
			16, 18, 19,
			20, 23, 22,
			20, 22, 21
		};

		std::vector<Vertex> vertices(cubeVerts, cubeVerts + sizeof(cubeVerts) / sizeof(Vertex));
		std::vector<GLuint> indices(cubeInds, cubeInds + sizeof(cubeInds) / sizeof(GLuint));

		Shape shape = { vertices, indices };
		return shape;
	}

	static Shape GetSquare() {
		Vertex vertices[] =
		{ //     COORDINATES     /        COLORS      
			glm::vec3(-0.5f, -0.5f, 0.0f),     glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(-0.5f,  0.5f, 0.0f),     glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.5f,  0.5f, 0.0f),      glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.5f, -0.5f, 0.0f),      glm::vec3(1.0f, 1.0f, 1.0f),
		};

		GLuint indices[] =
		{
			0, 2, 1, // Upper triangle
			0, 3, 2 // Lower triangle
		};
	}
};