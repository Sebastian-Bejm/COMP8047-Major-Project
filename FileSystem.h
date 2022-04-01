#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#include "MazeCell.h"

class FileSystem
{
public:
	static void WriteMazeDataToFile(std::vector<std::vector<MazeCell>> mazeCells);
	static std::vector<std::vector<MazeCell>> ReadMazeDataFile(std::string filename);

	static void WriteModelToFile(std::string filename);
	static void ReadModelFile(std::string filename);

};