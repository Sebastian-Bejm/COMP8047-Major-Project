#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <utility>

class FileSystem
{
public:
	static void WriteMazeDataToFile(std::string filename, std::string compact, std::string coded);
	static std::pair<std::vector<std::vector<std::string>>, 
		std::vector<std::vector<int>>> ReadMazeDataFile(std::string filename);

	static void WriteModelToFile(std::string filename);
	static void ReadModelFile(std::string filename);

};