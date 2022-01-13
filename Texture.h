#pragma once

#include <stb/stb_image.h>

#include "Shader.h"

class Texture
{
public:

	Texture();
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	void TexUnit(Shader shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();

private:

	GLuint ID;
	GLenum type;

};

