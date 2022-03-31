#include "TimeTracker.h"

TimeTracker& TimeTracker::GetInstance() {
	static TimeTracker instance;
	return instance;
}

void TimeTracker::StartTimer() {
	startTime = std::chrono::system_clock::now();
}

void TimeTracker::StopTimer() {
	if (!stopped) {
		stopped = true;
		endTime = std::chrono::system_clock::now();
	}
}

std::string TimeTracker::GetCurrentTime() {
	auto currentTime = std::chrono::system_clock::now();
	double elapsedTimeMS = std::chrono::duration<double, std::milli>(currentTime - startTime).count();
	double elapsedTimeSec = elapsedTimeMS / 1000;

	currentTimeStr = "Current Time: " + ConvertTimeToStr(elapsedTimeSec);
	return currentTimeStr;
}

std::string TimeTracker::GetLastBestTime() {
	std::string tempLast = lastBestTimeStr;

	if (stopped) {
		double elapsedTimeMS = std::chrono::duration<double, std::milli>(endTime - startTime).count();
		double elapsedTimeSec = elapsedTimeMS / 1000;

		tempLast = "Best Time: " + ConvertTimeToStr(elapsedTimeSec);
		lastBestTimeStr = tempLast;
		stopped = false;
	}

	return lastBestTimeStr;
}

std::string TimeTracker::ConvertTimeToStr(double elapsedTimeSec) {
	std::string convertedStr = "";

	if (elapsedTimeSec >= 60) {
		int mins = elapsedTimeSec / 60;
		double curSec = elapsedTimeSec - 60;
		convertedStr = std::to_string(mins) + "m " + std::to_string((int)curSec) + "s";
	}
	else {
		convertedStr = std::to_string((int)elapsedTimeSec) + "s";
	}

	return convertedStr;
}