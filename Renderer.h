#pragma once

#include <iostream>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Character.h"
#include "Time.h"
#include "Camera.h"
#include "ObjectTracker.h"
#include "ShapeDetails.h"

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

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	VAO vao;
	std::vector<Texture> textures;

	std::map<char, Character> characters;

	int windowWidth, windowHeight;
	glm::vec4 backgroundColour;

	void PrepareGLBuffers();
	void LoadTextures();
	void LoadFreetype();
	void RenderText();

	std::string GetTextureFileExtension(const std::string& textureFile);

	GLFWwindow* SetupGLFW();
};