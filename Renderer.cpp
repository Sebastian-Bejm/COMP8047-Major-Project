#include "Renderer.h"

Renderer* Renderer::renderer = nullptr;

GLFWwindow* window;

Renderer* Renderer::GetInstance() {
	if (renderer == nullptr) {
		renderer = new Renderer();
	}
	return renderer;
}

int Renderer::Init(glm::vec4 backgroundColour, int windowWidth, int windowHeight) {
	this->backgroundColour = backgroundColour;

	// Init GLFW
	glfwInit();

	// Tell GLFW what version we are using (OpenGL 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Setup our window for OpenGL
	window = glfwCreateWindow(windowWidth, windowHeight, "COMP8047 Major Project", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the window
	glViewport(0, 0, windowWidth, windowHeight);

	// Enable the depth buffer for 3D objects
	glEnable(GL_DEPTH_TEST);

	PrepareGLBuffers();

	CreateTextureBuffers();

	return 0;
}

void Renderer::PrepareGLBuffers() {
	// Get shape vertices and indices
	ShapeDetails shapeDetails;
	Shape shape = shapeDetails.GetShape(ShapeType::CUBE);

	this->vertices = shape.vertices;
	this->indices = shape.indices;

	vao.InitVAO();
	vao.Bind();
	// Generates Vertex Buffer object and links it to vertices
	VBO vbo = VBO(vertices);
	// Generates Element Buffer object and links it to indices
	EBO ebo = EBO(indices);

	// Links the attributes to the shader based on the layout
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // coordinates
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float))); // color
	vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float))); // texture

	// Unbind to prevent modification
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();

}

void Renderer::CreateTextureBuffers() {
	std::string crate = "crate.jpg";
	crateTexture = Texture(crate.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

	std::string brick = "brick.png";
	brickTexture = Texture(brick.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
}

int Renderer::Update(ObjectTracker* tracker) {
	// This fixes the fast movement over time on the machine
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Specify the color of the background (silver)
	glClearColor(backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a);

	// Clean the back buffer and assign the new color to it, and depth buffer for correct 3D rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.ProcessInput(window, deltaTime);
	camera.SetMatrix(45.0f, 0.1f, 100.0f);

	// Draw the game objects here with a reference to the camera
	std::vector<GameObject> objects = tracker->GetAllObjects();
	for (int i = 0; i < objects.size(); i++) {

		objects[i].GetShader().Activate();
		vao.Bind();

		if (objects[i].GetTag() == "crate") {
			crateTexture.TexUnit(objects[i].GetShader(), "tex0", 0);
			crateTexture.Bind();
		}
		else if (objects[i].GetTag() == "brick") {
			brickTexture.TexUnit(objects[i].GetShader(), "tex0", 0);
			brickTexture.Bind();
		}

		// Get our camera matrix in order to update the matrices to show where this new object is
		glm::mat4 cam = camera.GetCameraMatrix();
		GLint cameraLoc = glGetUniformLocation(objects[i].GetShader().GetID(), "camMatrix");
		glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(cam));

		// Draw the actual Mesh
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

		// Update the object model after drawing initial object
		GLint cubeLoc = glGetUniformLocation(objects[i].GetShader().GetID(), "model");
		glUniformMatrix4fv(cubeLoc, 1, GL_FALSE, glm::value_ptr(objects[i].GetTransform()->GetModelMatrix()));
	}

	glfwSwapBuffers(window);

	return 0;
}

int Renderer::Teardown() {
	vao.Delete();
	crateTexture.Delete();
	brickTexture.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void Renderer::SetCamera(Camera& camera) {
	this->camera = camera;
}

GLFWwindow* Renderer::SetupGLFW() {
	// Setup GLFW
	// Get the main monitor for the screen size
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* monitorInfo = glfwGetVideoMode(monitor);
	GLFWmonitor* fullScreenMonitor;

	glfwWindowHint(GLFW_RED_BITS, monitorInfo->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, monitorInfo->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, monitorInfo->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, monitorInfo->refreshRate);

	// Get the width and height of our monitor
	// We want to make it full screen
	windowWidth = monitorInfo->width;
	windowHeight = monitorInfo->height;
	fullScreenMonitor = monitor;

	// Create window with reference to the monitor in full size
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "COMP8047 Major Project", fullScreenMonitor, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create window" << std::endl;
		return NULL;
	}

	glfwMakeContextCurrent(window);

	delete monitor;
	delete monitorInfo;

	return window;
}

void Renderer::SetWindow(int width, int height) {
	if (width < 0 || height < 0) {
		return;
	}
	windowWidth = width;
	windowHeight = height;
}
