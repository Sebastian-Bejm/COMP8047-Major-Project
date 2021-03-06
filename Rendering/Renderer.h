#pragma once

#include <iostream>
#include <map>
#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"
#include "Texture.h"
#include "Character.h"
#include "Utility/Time.h"
#include "MainSystems/Camera.h"
#include "MainSystems/ObjectTracker.h"
#include "ShapeDetails.h"
#include "MainSystems/TimeTracker.h"
#include "MainSystems/GameManager.h"

extern GLFWwindow* window;

class Renderer {
public:

	static Renderer* GetInstance();

	int Init(glm::vec4 backgroundColor, int windowWidth, int windowHeight);
	int Update(ObjectTracker* tracker);
	int Teardown();

	void SetCamera(Camera& camera);
	void SetView(float left, float right, float bottom, float top);

private:

	static Renderer* renderer;
	Camera camera;

	VAO vao;
	VBO textVBO;

	// Vectors for the vertices and indices of each game object
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	// Vector for loading textures on initialization
	std::vector<Texture> textures; 

	// Variables for handling freetype text
	std::map<char, Character> characters;
	glm::mat4 textProjectionMatrix;
	Shader textShader;

	// Variables for window size and color
	int windowWidth = 0, windowHeight = 0;
	glm::vec4 backgroundColour;

	// Variables for orthographic view
	float viewLeft = 0.0f, viewRight = 0.0f, viewBottom = 0.0f, viewTop = 0.0f;

	const int TEXT_WIDTH_OFFSET = 500;
	const int TEXT_HEIGHT_OFFSET = 80;

	void PrepareGLBuffers();
	void LoadTextures();
	void LoadFreetype();
	void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

	std::string GetTextureFileExtension(const std::string& textureFile);

};