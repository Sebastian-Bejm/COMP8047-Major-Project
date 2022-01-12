#include "Renderer.h"

Renderer* Renderer::renderer = nullptr;

int Renderer::Init(int viewWidth, int viewHeight, glm::vec4 backgroundColor) {
	this->backgroundColor = backgroundColor;
}

int Renderer::Update() {

}

int Renderer::Teardown() {

}

void Renderer::SetWindow(int height, int width) {

}

void Renderer::DrawObjects() {
	// ObjecTracker here
}
