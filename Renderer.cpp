#include "Renderer.h"

Renderer* Renderer::renderer = nullptr;

Renderer* Renderer::GetInstance() {
	if (renderer == nullptr) {
		renderer = new Renderer();
	}
	return renderer;
}

int Renderer::Init(glm::vec4 backgroundColour) {
	this->backgroundColour = backgroundColour;

	// Init GLFW
	glfwInit();

	// Tell GLFW what version we are using (OpenGL 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Setup our window for OpenGL
	window = SetupGLFW();
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the window
	glViewport(0, 0, windowWidth, windowHeight);

	// Enable the depth buffer for 3D objects
	glEnable(GL_DEPTH_TEST);

	return 0;
}

int Renderer::Update(ObjectTracker* tracker) {

	// Specify the color of the background (silver)
	glClearColor(backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a);

	// Clean the back buffer and assign the new color to it, and depth buffer for correct 3D rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw the game objects here with a reference to the camera
	std::vector<GameObject> objects = tracker->GetAllObjects();
	for (int i = 0; i < objects.size(); i++) {
		objects[i].Draw(camera);
	}

	glfwSwapBuffers(window);

	return 0;
}

int Renderer::Teardown() {

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
