#include "Texture.h"

Texture::Texture() {}

Texture::Texture(const char* image, const char* texType, GLenum slot, GLenum format, GLenum pixelType) {
	type = texType;

	// Stores the width, height, and number of color channels in the image
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true); // Puts the image rightside up
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// Generate the texture object
	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Configures the type of algorithm used to make image smaller or bigger
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configure how the texture repeats
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// In case I need to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	// Assigns the image to the texture object
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	// Generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Delete the image data since it's already in a texture object
	stbi_image_free(bytes);
	// Unbind the texture object so it can't be modified
	glBindTexture(GL_TEXTURE_2D, 0);
}

std::string Texture::GetType() {
	return type;
}


void Texture::TexUnit(Shader shader, const char* uniform, GLuint unit) {
	GLuint texUni = glGetUniformLocation(shader.GetID(), uniform);

	shader.Activate();

	glUniform1i(texUni, unit);
}

void Texture::Bind() {
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}
