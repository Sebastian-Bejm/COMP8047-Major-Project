#pragma once

#include <GLFW/glfw3.h>

class Time {
public:

	static Time& GetInstance() {
		static Time instance;
		return instance;
	}

	float DeltaTime() {
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		return deltaTime;
	}

private:
	double deltaTime;
	double currentFrame, lastFrame;
};