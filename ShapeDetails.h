#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "Vertex.h"

struct Shape {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
};

enum ShapeType {
	SQUARE,
	CUBE,
	PYRAMID,
};

class ShapeDetails {
public:

	ShapeDetails() {}
	
	Shape GetShape(ShapeType shapeType) {
		Shape shape;

		switch (shapeType) {
		case SQUARE:
			shape = GetSquare();
			break;
		case CUBE:
			shape = GetCube();
			break;
		case PYRAMID:
			shape = GetPyramid();
			break;
		default:
			break;
		}

		return shape;
	}

private:

	// https://learnopengl.com/Advanced-OpenGL/Cubemaps

	Shape GetCube() {
		// 24
		Vertex cubeVerts[] = {
			//		COORDINATES					,			COLORS			,
			glm::vec3(-0.5f, -0.5f, -0.5f),		glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(0.0f, 0.0f),
			glm::vec3(-0.5f, -0.5f,  0.5f),		glm::vec3(0.95f, 0.85f, 0.7f),	glm::vec2(0.0f, 1.0f),
			glm::vec3(0.5f, -0.5f,  0.5f),		glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(1.0f, 1.0f),
			glm::vec3(0.5f, -0.5f, -0.5f),		glm::vec3(0.95f, 0.85f, 0.7f),	glm::vec2(1.0f, 0.0f),

			glm::vec3(-0.5f,  0.5f, -0.5f),		glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(0.0f, 0.0f),
			glm::vec3(-0.5f,  0.5f,  0.5f),		glm::vec3(0.95f, 0.85f, 0.7f),	glm::vec2(0.0f, 1.0f),
			glm::vec3(0.5f,  0.5f,  0.5f),		glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(1.0f, 1.0f),
			glm::vec3(0.5f,  0.5f, -0.5f),		glm::vec3(0.95f, 0.85f, 0.7f),	glm::vec2(1.0f, 0.0f),

			glm::vec3(-0.5f, -0.5f, -0.5f),		glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(0.0f, 0.0f),
			glm::vec3(-0.5f,  0.5f, -0.5f),		glm::vec3(0.95f, 0.85f, 0.7f),	glm::vec2(0.0f, 1.0f),
			glm::vec3(0.5f,  0.5f, -0.5f),		glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(1.0f, 1.0f),
			glm::vec3(0.5f, -0.5f, -0.5f),		glm::vec3(0.95f, 0.85f, 0.7f),	glm::vec2(1.0f, 0.0f),

			glm::vec3(-0.5f, -0.5f, 0.5f),		glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(0.0f, 0.0f),
			glm::vec3(-0.5f,  0.5f, 0.5f),		glm::vec3(0.95f, 0.85f, 0.7f),	glm::vec2(0.0f, 1.0f),
			glm::vec3(0.5f,  0.5f, 0.5f),		glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(1.0f, 1.0f),
			glm::vec3(0.5f, -0.5f, 0.5f),		glm::vec3(0.95f, 0.85f, 0.7f),	glm::vec2(1.0f, 0.0f),

			glm::vec3(-0.5f, -0.5f, -0.5f),		glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(0.0f, 0.0f),
			glm::vec3(-0.5f, -0.5f,  0.5f),		glm::vec3(0.95f, 0.85f, 0.7f),	glm::vec2(0.0f, 1.0f),
			glm::vec3(-0.5f,  0.5f,  0.5f),		glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(1.0f, 1.0f),
			glm::vec3(-0.5f,  0.5f, -0.5f),		glm::vec3(0.95f, 0.85f, 0.7f),	glm::vec2(1.0f, 0.0f),

			glm::vec3(0.5f, -0.5f, -0.5f),		glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(0.0f, 0.0f),
			glm::vec3(0.5f, -0.5f,  0.5f),		glm::vec3(0.95f, 0.85f, 0.7f),	glm::vec2(0.0f, 1.0f),
			glm::vec3(0.5f,  0.5f,  0.5f),		glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(1.0f, 1.0f),
			glm::vec3(0.5f,  0.5f, -0.5f),		glm::vec3(0.95f, 0.85f, 0.7f),	glm::vec2(1.0f, 0.0f)
		};

		/*glm::vec3 vertices[] =
		{
			glm::vec3(-1, -1, -1),
			glm::vec3(1, -1, -1),
			glm::vec3(1, 1, -1),
			glm::vec3(-1, 1, -1),

			glm::vec3(-1, -1, 1),
			glm::vec3(1, -1, 1),
			glm::vec3(1, 1, 1),
			glm::vec3(-1, 1, 1)
		};

		glm::vec2 texCoords[] =
		{
			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(1, 1),
			glm::vec2(0, 1)
		};

		Vector3f normals[6] =
		{
			Vector3f(0, 0, 1),
			Vector3f(1, 0, 0),
			Vector3f(0, 0, -1),
			Vector3f(-1, 0, 0),
			Vector3f(0, 1, 0),
			Vector3f(0, -1, 0)
		};

		GLint indices[6 * 6] =
		{
			0, 1, 3, 3, 1, 2,
			1, 5, 2, 2, 5, 6,
			5, 4, 6, 6, 4, 7,
			4, 0, 7, 7, 0, 3,
			3, 2, 7, 7, 2, 6,
			4, 5, 0, 0, 5, 1
		};*/

		int cubeInds[] = {
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

	Shape GetSquare() {
		Vertex squareVerts[] =
		{	//     COORDINATES				/			COLORS				/		TEXTURES
			glm::vec3(-0.5f, -0.5f, 0.0f),     glm::vec3(1.0f, 0.0f, 0.0f),		glm::vec2(0.0f, 0.0f),
			glm::vec3(-0.5f,  0.5f, 0.0f),     glm::vec3(0.0f, 1.0f, 0.0f),		glm::vec2(0.0f, 1.0f),
			glm::vec3(0.5f,  0.5f, 0.0f),      glm::vec3(0.0f, 0.0f, 1.0f),		glm::vec2(1.0f, 1.0f),
			glm::vec3(0.5f, -0.5f, 0.0f),      glm::vec3(1.0f, 1.0f, 1.0f),		glm::vec2(1.0f, 0.0f)
		};

		GLuint squareInds[] =
		{
			0, 2, 1, // Upper triangle
			0, 3, 2 // Lower triangle
		};

		std::vector<Vertex> vertices(squareVerts, squareVerts + sizeof(squareVerts) / sizeof(Vertex));
		std::vector<GLuint> indices(squareInds, squareInds + sizeof(squareInds) / sizeof(GLuint));

		Shape shape = { vertices, indices };
		return shape;
	}

	Shape GetPyramid() {
		Vertex pyramidVerts[] =
		{
			glm::vec3(-0.5f, 0.0f, 0.5f),	glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(0.0f, 0.0f), // Lower left corner
			glm::vec3(-0.5f, 0.0f, -0.5f),	glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(1.0f, 0.0f), // Upper left corner
			glm::vec3(0.5f, 0.0f, 0.5f),	glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(1.0f, 0.0f),// Lower right corner
			glm::vec3(0.0f, 0.8f, 0.0f),	glm::vec3(0.95f, 0.85f, 0.7f),	glm::vec2(0.5f, 1.0f),// Lower left corner
		};

		GLuint pyramidInds[] = {
			0, 1, 2,
			0, 2, 3,
			0, 1, 4,
			1, 2, 4,
			2, 3, 4,
			3, 0, 4
		};

		std::vector<Vertex> vertices(pyramidVerts, pyramidVerts + sizeof(pyramidVerts) / sizeof(Vertex));
		std::vector<GLuint> indices(pyramidInds, pyramidInds + sizeof(pyramidInds) / sizeof(GLuint));

		Shape shape = { vertices, indices };
		return shape;
	}
};