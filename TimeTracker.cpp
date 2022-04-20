#include "TimeTracker.h"

TimeTracker& TimeTracker::GetInstance() {
	static TimeTracker instance;
	return instance;
}

void TimeTracker::StartTimer() {
	stopped = false;
	startTime = std::chrono::system_clock::now();
}

void TimeTracker::StopTimer() {
	if (!stopped) {
		endTime = std::chrono::system_clock::now();
		stopped = true;
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

		currentBestTime = elapsedTimeSec;
		if (localBestTime == 0.0) {
			localBestTime = currentBestTime;
		}
		else if (currentBestTime <= localBestTime) {
			localBestTime = currentBestTime;
		}
		tempLast = "Best Time: " + ConvertTimeToStr(localBestTime);
		lastBestTimeStr = tempLast;
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