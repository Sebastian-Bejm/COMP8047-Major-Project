#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:

	Shader();
	Shader(const char* vertexFile, const char* fragmentFile);

	GLuint GetID();

	void Activate();
	void Delete();

private:

	GLuint ID;

	std::string GetFileContents(const char* filename);

	void CompileErrors(unsigned int shader, const char* type);
};