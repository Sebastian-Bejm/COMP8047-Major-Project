#pragma once

#include <glad/glad.h>
#include "Utility/GLMHeader.h"

// Text character structure
struct Character {
	GLuint textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};