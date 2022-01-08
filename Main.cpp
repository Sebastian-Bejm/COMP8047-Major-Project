#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Mesh.h"

const int screenWidth = 1000;
const int screenHeight = 800;


// Pyramid
/*GLfloat pyramidVertices[] =
{
	-0.5f, 0.0f, 0.5f,	0.8f, 0.7f, 0.4f, // Lower left corner
	-0.5f, 0.0f, -0.5f,	0.8f, 0.7f, 0.4f, // Upper left corner
	0.5f, 0.0f, -0.5f,	0.8f, 0.7f, 0.4f, // Upper right corner
	0.5f, 0.0f, 0.5f,	0.8f, 0.7f, 0.4f, // Lower right corner
	0.0f, 0.8f, 0.0f,	0.95f, 0.85f, 0.7f, // Lower left corner
};*/

Vertex pyramidVertices[] =
{
	glm::vec3(-0.5f, 0.0f, 0.5f),	glm::vec3(0.8f, 0.7f, 0.4f), // Lower left corner
	glm::vec3(-0.5f, 0.0f, -0.5f),	glm::vec3(0.8f, 0.7f, 0.4f), // Upper left corner
	glm::vec3(0.5f, 0.0f, -0.5f),	glm::vec3(0.8f, 0.7f, 0.4f), // Upper right corner
	glm::vec3(0.5f, 0.0f, 0.5f),	glm::vec3(0.8f, 0.7f, 0.4f), // Lower right corner
	glm::vec3(0.0f, 0.8f, 0.0f),	glm::vec3(0.95f, 0.85f, 0.7f), // Lower left corner
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
Vertex cubeVertices[] = {
	glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
	glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
	glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
	glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
	glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
	glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
	glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
	glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
	glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
	glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
	glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
	glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
	glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
	glm::vec3(-0.5f,  0.5f, 0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
	glm::vec3(0.5f,  0.5f, 0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
	glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
	glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
	glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
	glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
	glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
	glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
	glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
	glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.8f, 0.7f, 0.4f),
	glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.95f, 0.85f, 0.7f),
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

// TODO: refactor later for initalize, update, and teardown
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
	Shader cubeShader("DefaultVertShader.vs", "DefaultFragShader.fs");

	std::vector<Vertex> vertices(pyramidVertices, pyramidVertices + sizeof(pyramidVertices) / sizeof(Vertex));
	std::vector<GLuint> indices(pyramidIndices, pyramidIndices + sizeof(pyramidIndices) / sizeof(GLuint));

	std::vector<Vertex> cVerts(cubeVertices, cubeVertices + sizeof(cubeVertices) / sizeof(Vertex));
	std::vector<GLuint> cInds(cubeIndices, cubeIndices + sizeof(cubeIndices) / sizeof(GLuint));

	// Create new mesh with vertices and indices
	Mesh pyramidMesh(vertices, indices);
	Mesh cubeMesh(cVerts, cInds);

	// Set up model matrixes
	glm::mat4 pyrModel = glm::mat4(1.0f);

	glm::vec3 cubePosition(1.0f, 0.5, 0.0f);

	glm::mat4 cubeModel = glm::mat4(1.0f);
	cubeModel = glm::translate(glm::vec3(cubePosition));

	// Tell OpenGL what Shader program we want to use
	shaderProgram.Activate();
	GLint modelLoc = glGetUniformLocation(shaderProgram.GetID(), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pyrModel));

	cubeShader.Activate();
	GLint cubeLoc = glGetUniformLocation(cubeShader.GetID(), "model");
	glUniformMatrix4fv(cubeLoc, 1, GL_FALSE, glm::value_ptr(cubeModel));



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

		camera.ProcessInput(window);
		camera.SetMatrix(45.0f, 0.1f, 100.0f);

		pyramidMesh.Draw(shaderProgram, camera); 
		cubeMesh.Draw(cubeShader, camera);

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			cubePosition.x -= 0.1f;
			
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			cubePosition.x += 0.1f;
		}
		cubeModel = glm::translate(glm::vec3(cubePosition));

		GLint cubeLoc = glGetUniformLocation(cubeShader.GetID(), "model");
		glUniformMatrix4fv(cubeLoc, 1, GL_FALSE, glm::value_ptr(cubeModel));



		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		// todo create Update functions for rendering, physics, manager, etc.
		glfwPollEvents();
	}

	// Cleanup objects we have created
	shaderProgram.Delete();
	cubeShader.Delete();

	// Destroy window when done and exit
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}