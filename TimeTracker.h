#pragma once

#include <string>

class TimeTracker
{
public:

	static TimeTracker& GetInstance();

	void NextFrame();
	void StopTimer(bool stop);

	std::string GetCurrentTime();
	std::string GetLastBestTime();

private:

	float deltaTime;
	float currentTime, prevFrame;
	float lastBestTime;
};

