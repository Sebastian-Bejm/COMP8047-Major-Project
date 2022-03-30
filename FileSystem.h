#pragma once

#include <string>
#include <fstream>

class FileSystem
{
public:
	static void WriteMazeDataToFile(std::string mazeData);
	static std::string ReadMazeDataFile(std::string filename);
	static void WriteModelToFile();
	static void ReadModelFile(); // return model in binary?
};

