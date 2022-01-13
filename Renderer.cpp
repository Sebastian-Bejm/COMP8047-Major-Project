#include "Renderer.h"

Renderer* Renderer::renderer = nullptr;

int Renderer::Init(int viewWidth, int viewHeight, glm::vec4 backgroundColor) {
	this->backgroundColor = backgroundColor;

	return 0;
}

int Renderer::Update() {

	return 0;
}

int Renderer::Teardown() {
	return 0;
}

void Renderer::SetWindow(int height, int width) {

}

void Renderer::DrawObjects() {
	// ObjecTracker here
}
