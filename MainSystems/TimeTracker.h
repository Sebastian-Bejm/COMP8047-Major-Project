#pragma once

#include <string>
#include <chrono>
#include <iostream>

class TimeTracker
{
public:

	static TimeTracker& GetInstance();

	void StartTimer();
	void StopTimer();

	auto GetCurrentTime();

	std::string GetCurrentTimeString();
	std::string GetLastBestTimeString();

private:

	bool stopped = false;
	std::chrono::system_clock::time_point startTime, endTime;
	double localBestTime, currentBestTime;

	std::string currentTimeStr = "";
	std::string lastBestTimeStr = "Best Time: ";

	std::string ConvertTimeToStr(double elapsedTimeSec);
};

