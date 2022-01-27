#pragma once

#include <chrono>
#include <iostream>

class FPSCounter
{
public:

	FPSCounter();
	void NextFrame();

private:
	unsigned int framesThisSecond;
	clock_t prevTime;
	clock_t deltaTime = 0;
};

