#pragma once

#include <iostream>
#include <map>
#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Character.h"
#include "Time.h"
#include "Camera.h"
#include "ObjectTracker.h"
#include "ShapeDetails.h"
#include "TimeTracker.h"

extern GLFWwindow* window;

class Renderer {
public:

	static Renderer* GetInstance();

	int Init(glm::vec4 backgroundColor, int windowWidth, int windowHeight);
	int Update(ObjectTracker* tracker); // ObjectTracker will be added here
	int Teardown();

	void SetCamera(Camera& camera);

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

	// Store the freetype characters
	std::map<char, Character> characters;
	glm::mat4 textProjectionMatrix;
	Shader textShader;

	// Variables for window size and color
	int windowWidth, windowHeight;
	glm::vec4 backgroundColour;

	void PrepareGLBuffers();
	void LoadTextures();
	void LoadFreetype();
	void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

	std::string GetTextureFileExtension(const std::string& textureFile);

};