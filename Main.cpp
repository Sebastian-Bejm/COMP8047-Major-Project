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
#include "Camera.h"

const int screenWidth = 1000;
const int screenHeight = 800;

// Moving these up here - temporary until i learn about colour buffer
// first 3 are x y z, next 3 are r g b
/*GLfloat vertices[] =
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
};*/

// Pyramid
GLfloat pyramidVertices[] =
{
	-0.5f, 0.0f, 0.5f,	0.8f, 0.7f, 0.4f, // Lower left corner
	-0.5f, 0.0f, -0.5f,	0.8f, 0.7f, 0.4f, // Upper left corner
	0.5f, 0.0f, -0.5f,	0.8f, 0.7f, 0.4f, // Upper right corner
	0.5f, 0.0f, 0.5f,	0.8f, 0.7f, 0.4f, // Lower right corner
	0.0f, 0.8f, 0.0f,	0.95f, 0.85f, 0.7f, // Lower left corner
};

GLuint pyramidIndices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

// Cube
GLfloat cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f,  0.5f, 0.5f,
	0.5f,  0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f, -0.5f,
};

GLint cubeIndices[] = {
	0, 2, 1,
	0, 3, 2,
	4, 5, 6,
	4, 6, 7,
	8, 9, 10,
	8, 10, 11,
	12, 15, 14,
	12, 14, 13,
	16, 17, 18,
	16, 18, 19,
	20, 23, 22,
	20, 22, 21
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
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "COMP 8047 Major Project", NULL, NULL);
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
	glViewport(0, 0, screenWidth, screenHeight);

	Shader shaderProgram("DefaultVertShader.vs", "DefaultFragShader.fs");

	// Create the VAO, VBO, and EBO for the current rendered object (must be in this order)
	VAO vao;
	vao.Bind();

	VBO vbo(pyramidVertices, sizeof(pyramidVertices));
	EBO ebo(pyramidIndices, sizeof(pyramidIndices));

	int numVertices = sizeof(pyramidVertices) / sizeof(pyramidVertices[0]);
	int numIndices = sizeof(pyramidIndices) / sizeof(pyramidIndices[0]);

	// temporary until I learn to set these params dynamically based on object info
	// Links the attributes to the shader based on layout
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

	// Enable the depth buffer for 3D objects
	glEnable(GL_DEPTH_TEST);

	// Set up the camera
	Camera camera(screenWidth, screenHeight, glm::vec3(0.0f, 0.5f, 2.0f));

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		// Specify the color of the background (silver)
		glClearColor(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL what Shader program we want to use
		shaderProgram.Activate();

		camera.ProcessInput(window);
		camera.SetMatrix(45.0f, 0.1f, 100.0f);

		// Get the model, view, and projection matrices
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 proj = camera.GetProjectionMatrix();

		// Set the uniforms in the shader
		GLint modelLoc = glGetUniformLocation(shaderProgram.GetID(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GLint viewLoc = glGetUniformLocation(shaderProgram.GetID(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		GLint projLoc = glGetUniformLocation(shaderProgram.GetID(), "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		// Bind the VAO so OpenGL knows to use it
		vao.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices (this would be when looping to render multiple objects)
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
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