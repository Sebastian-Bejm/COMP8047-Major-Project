#include "Renderer.h"

Renderer* Renderer::renderer = nullptr;

GLFWwindow* window;

int Renderer::Init(int viewWidth, int viewHeight, glm::vec4 backgroundColor) {
	this->backgroundColor = backgroundColor;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Get the main monitor for the screen size
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* monitorInfo = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, monitorInfo->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, monitorInfo->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, monitorInfo->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, monitorInfo->refreshRate);

	return 0;
}

int Renderer::Update() {

	return 0;
}

int Renderer::Teardown() {

	glfwDestroyWindow(window);


	return 0;
}

Renderer* Renderer::GetInstance() {
	if (renderer == nullptr) {
		renderer = new Renderer();
	}
	return renderer;
}


void Renderer::SetWindow(int height, int width) {
	if (height < 0 || width < 0) {
		return;
	}
	windowHeight = height;
	windowWidth = width;
}

void Renderer::DrawObjects() {
	// ObjecTracker here
}
