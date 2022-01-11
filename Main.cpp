#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Texture.h"
#include "GameObject.h"

const int screenWidth = 1000;
const int screenHeight = 800;

float deltaTime;
float currentFrame, lastFrame;

// TODO: refactor later for initalize, update, and teardown
int Initialize() {
	return 0;
}

void Teardown() {
	
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

	//Shader shaderProgram("DefaultVertShader.vs", "DefaultFragShader.fs");
	//Shader cubeShader("DefaultVertShader.vs", "DefaultFragShader.fs");

	//Shader gameobjectShader("DefaultVertShader.vs", "DefaultFragShader.fs");

	//std::vector<Vertex> vertices(pyramidVertices, pyramidVertices + sizeof(pyramidVertices) / sizeof(Vertex));
	//std::vector<GLuint> indices(pyramidIndices, pyramidIndices + sizeof(pyramidIndices) / sizeof(GLuint));

	// Create new mesh with vertices and indices
	//Mesh pyramidMesh(vertices, indices);

	// Set up model matrixes for each new object
	//glm::mat4 pyrModel = glm::mat4(1.0f);

	//glm::mat4 cubeModel = glm::mat4(1.0f);
	//glm::vec3 cubePosition(1.0f, 0.5f, 0.0f);
	// Transform for the cube - temp here
	//Transform transform(cubePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// Tell OpenGL what shader programs we want to use
	//shaderProgram.Activate();
	//GLint modelLoc = glGetUniformLocation(shaderProgram.GetID(), "model");
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pyrModel));
	//GameObject middleCube("MIDDLE_CUBE", CUBE, shaderProgram, glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	//cubeShader.Activate();
	//GLint cubeLoc = glGetUniformLocation(cubeShader.GetID(), "model");
	//glUniformMatrix4fv(cubeLoc, 1, GL_FALSE, glm::value_ptr(cubeModel));
	//GameObject firstCube("FCUBE", CUBE, cubeShader, glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// testing new cube object
	//gameobjectShader.Activate();
	//GameObject cubeObject("CUBE", CUBE, gameobjectShader, glm::vec3(-1.0f, 0.5, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	Shader textureShader("TextureVertShader.vs", "TextureFragShader.fs");

	ShapeDetails shapeDetails;
	Shape pyramid = shapeDetails.GetShape(PYRAMID);

	// vectors are already sized correctly
	std::vector<Vertex> verts = pyramid.vertices;
	std::vector<GLuint> inds = pyramid.indices;

	Texture texture("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	texture.TexUnit(textureShader, "tex0", 0);

	Mesh mesh(verts, inds, texture);

	textureShader.Activate();
	//GLint cubeLoc = glGetUniformLocation(textureShader.GetID(), "model");
	//glUniformMatrix4fv(cubeLoc, 1, GL_FALSE, glm::value_ptr(cubeModel));

	//GameObject texPyramid("TexPyrm", "brick.png", PYRAMID, textureShader,
	//	glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

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
		// This fixes objects/entities moving too fast
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Specify the color of the background (silver)
		glClearColor(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.ProcessInput(window, deltaTime);
		camera.SetMatrix(45.0f, 0.1f, 100.0f);

		glm::mat4 model = glm::mat4(1.0f);

		GLint modelLoc = glGetUniformLocation(textureShader.GetID(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		mesh.Draw(textureShader, camera);

		/*if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			//transform.Translate(glm::vec3(-0.5f, 0.0f, 0.0f), deltaTime);
			//texPyramid.GetTransform().Translate(glm::vec3(-0.5f, 0.0f, 0.0f), deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			//transform.Translate(glm::vec3(0.5f, 0.0f, 0.0f), deltaTime);
			//texPyramid.GetTransform().Translate(glm::vec3(0.5f, 0.0f, 0.0f), deltaTime);
		}*/

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		// todo create Update functions for rendering, physics, manager, etc.
		glfwPollEvents();
	}

	// Cleanup objects we have created
	mesh.Delete();
	textureShader.Delete();

	// Destroy window when done and exit
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}