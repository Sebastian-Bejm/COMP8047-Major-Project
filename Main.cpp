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

// Pyramid
Vertex pyramidVerts[] =
{
	glm::vec3(-0.5f, 0.0f, 0.5f),	glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(0.0f, 0.0f), // Lower left corner
	glm::vec3(-0.5f, 0.0f, -0.5f),	glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(5.0f, 0.0f), // Upper left corner
	glm::vec3(0.5f, 0.0f, -0.5f),	glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(0.0f, 0.0f), // Upper right corner
	glm::vec3(0.5f, 0.0f, 0.5f),	glm::vec3(0.8f, 0.7f, 0.4f),	glm::vec2(5.0f, 0.0f),// Lower right corner
	glm::vec3(0.0f, 0.8f, 0.0f),	glm::vec3(0.95f, 0.85f, 0.7f),	glm::vec2(2.5f, 5.0f),// Lower left corner
};

GLfloat pyramidVertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

GLuint pyramidIndices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

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

	// already sized
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

	// Generates Vertex Array Object and binds it
	/*VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(pyramidVertices, sizeof(pyramidVertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(pyramidIndices, sizeof(pyramidIndices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	Texture texture("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	texture.TexUnit(textureShader, "tex0", 0);*/

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
		//glm::mat4 cam = camera.GetCameraMatrix();

		GLint modelLoc = glGetUniformLocation(textureShader.GetID(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//GLint camLoc = glGetUniformLocation(textureShader.GetID(), "camMatrix");
		//glUniformMatrix4fv(camLoc, 1, GL_FALSE, glm::value_ptr(cam));

		mesh.Draw(textureShader, camera);

		/*if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			//transform.Translate(glm::vec3(-0.5f, 0.0f, 0.0f), deltaTime);
			//texPyramid.GetTransform().Translate(glm::vec3(-0.5f, 0.0f, 0.0f), deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			//transform.Translate(glm::vec3(0.5f, 0.0f, 0.0f), deltaTime);
			//texPyramid.GetTransform().Translate(glm::vec3(0.5f, 0.0f, 0.0f), deltaTime);
		}*/

		//texPyramid.Update(camera);

		//texSquare.Update(camera);

		//cubeObject.Update(camera); // the leftmost
		//middleCube.Update(camera); 
		//firstCube.Update(camera); // the right

		//GLint cubeLoc = glGetUniformLocation(cubeShader.GetID(), "model");
		//glUniformMatrix4fv(cubeLoc, 1, GL_FALSE, glm::value_ptr(transform.GetModelMatrix()));

		//pyramidMesh.Draw(shaderProgram, camera); 
		//cubeMesh.Draw(cubeShader, camera);

		//textureShader.Activate();

		//texture.Bind();

		//VAO1.Bind();

		//glDrawElements(GL_TRIANGLES, sizeof(pyramidIndices)/sizeof(int), GL_UNSIGNED_INT, 0);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		// todo create Update functions for rendering, physics, manager, etc.
		glfwPollEvents();
	}

	// Cleanup objects we have created
	//shaderProgram.Delete();
	//cubeShader.Delete();

	//middleCube.Delete();
	//firstCube.Delete();
	//cubeObject.Delete();

	//texPyramid.Delete();

	mesh.Delete();
	textureShader.Delete();

	/*VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	texture.Delete();
	textureShader.Delete();*/

	// Destroy window when done and exit
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}