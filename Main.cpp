#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

// Moving these up here - temporary until i learn about colour buffer
// first 3 are x y z, next 3 are r g b
GLfloat vertices[] =
{
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,	0.8f, 0.3f, 0.02f, // Lower left corner
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,		0.8f, 0.3f, 0.02f, // Lower right corner
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,	0.8f, 0.3f, 0.02f, // Upper corner
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, 0.8f, 0.3f, 0.02f,
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,	0.8f, 0.3f, 0.02f,
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f,		0.8f, 0.3f, 0.02f,
};

GLuint indices[] = {
	0, 3, 5,
	3, 2, 4,
	5, 4, 1
};

// TODO: refactor later fot initalize, update, and teardown
int initialize() {
	return 0;
}

void teardown() {
	
}

int main() {

	// Init GLFW
	glfwInit();

	// Tell GLFW what version we are using (OpenGL 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create GLFW window
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "COMP 8047 Major Project", NULL, NULL);
	// If window fails to create it exits
	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the window
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader shaderProgram("DefaultVertShader.vs", "DefaultFragShader.fs");

	// Create the VAO, VBO, and EBO for the current rendered object (must be in this order)
	VAO vao;
	vao.Bind();

	VBO vbo(vertices, sizeof(vertices));
	EBO ebo(indices, sizeof(indices));

	// temporary until I learn to use colour buffer seperately
	// Links the attributes to the shader based on layout (2nd param)
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3*sizeof(float)));

	// Unbind to prevent modification
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();

	// Specify the color of the background (silver)
	glClearColor(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		// Specify the color of the background (silver)
		glClearColor(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL what Shader program we want to use
		shaderProgram.Activate();
		
		glm::mat4 model = glm::mat4(1.0f);

		// Bind the VAO so OpenGL knows to use it
		vao.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices (this would be when looping to render multiple objects)
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		// todo create Update functions for rendering, physics, manager, etc.
		glfwPollEvents();
	}

	// Cleanup objects we have created
	vao.Delete();
	vbo.Delete();
	ebo.Delete();
	shaderProgram.Delete();

	// Destroy window when done and exit
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}