#pragma once

// NOTE: May have to use #ifndef for each of the recent classes

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:

	Shader(const char* vertexFile, const char* fragmentFile) {
		std::string vertexCode = GetFileContents(vertexFile);
		std::string fragmentCode = GetFileContents(fragmentFile);

		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();

		// Create basic shaders for a new object
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		CompileErrors(vertexShader, "VERTEX");

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		CompileErrors(fragmentShader, "FRAGMENT");

		// Attatch the shaders to the program
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		// Link all shaders to the program
		glLinkProgram(ID);
		CompileErrors(ID, "PROGRAM");

		// Now that they are attached they can be deleted
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}

	void Activate() {
		glUseProgram(ID);
	}

	void Delete() {
		glDeleteProgram(ID);
	}

private:

	GLuint ID;

	std::string GetFileContents(const char* filename) {
		std::ifstream in(filename, std::ios::binary);

		if (in) {
			std::string contents;

			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);

			in.read(&contents[0], contents.size());
			in.close();

			return contents;
		}
		else {
			std::cerr << "Shader contents cannot be read" << std::endl;
		}
		throw(errno);

	}

	// Checks if the different Shaders have compiled properly
	void CompileErrors(unsigned int shader, const char* type)
	{
		// Stores status of compilation
		GLint hasCompiled;
		// Character array to store error message in
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
			if (hasCompiled == GL_FALSE)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
			if (hasCompiled == GL_FALSE)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
			}
		}
	}
};