#include "Texture.h"

Texture::Texture() {}

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
	type = texType;

	// Stores the width, height, and number of color channels in the image
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true); // Puts the image rightside up
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// Generate the texture object
	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Configures the type of algorithm used to make image smaller or bigger
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Configure how the texture repeats
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// In case I need to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(texType GL_TEXTURE_BORDER_COLOR, flatColor);

	// Assigns the image to the texture object
	glTexImage2D(texType, 0, format, widthImg, heightImg, 0, format, pixelType, bytes);
	// Generate the mipmaps
	glGenerateMipmap(texType);

	// Delete the image data since it's already in a texture object
	stbi_image_free(bytes);
	// Unbind the texture object so it can't be modified
	glBindTexture(texType, 0);
}

void Texture::TexUnit(Shader shader, const char* uniform, GLuint unit) {
	GLuint texUni = glGetUniformLocation(shader.GetID(), uniform);

	shader.Activate();

	glUniform1i(texUni, unit);
}

void Texture::Bind() {
	glBindTexture(type, ID);
}

void Texture::Unbind() {
	glBindTexture(type, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}
