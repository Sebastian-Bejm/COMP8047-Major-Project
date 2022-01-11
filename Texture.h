#pragma once

#include <stb/stb_image.h>
#include <glad/glad.h>

#include "Shader.h"

class Texture
{
public:

	Texture();
	Texture(const char* image, const char* texType, GLenum slot, GLenum format, GLenum pixelType);

	std::string GetType();

	void TexUnit(Shader shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();

private:

	GLuint ID;
	GLuint unit;
	const char* type;

};

