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

	std::string GetCurrentTime();
	std::string GetLastBestTime();

private:

	bool stopped = false;
	std::chrono::system_clock::time_point startTime, endTime;
	std::string currentTimeStr = "";
	std::string lastBestTimeStr = "Best Time: ";

	std::string ConvertTimeToStr(double elapsedTimeSec);
};

