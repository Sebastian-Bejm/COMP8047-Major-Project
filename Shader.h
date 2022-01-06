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
		std::string vertexCode = get_file_contents(vertexFile);
		std::string fragmentCode = get_file_contents(fragmentFile);

		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();

		// Create basic shaders for this 2D object - tutorial
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);

		// Attatch the shaders to the program
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		// Link all shaders to the program
		glLinkProgram(ID);

		// Now that they are attached they can be deleted to save memory
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

	std::string get_file_contents(const char* filename) {
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

	}
};