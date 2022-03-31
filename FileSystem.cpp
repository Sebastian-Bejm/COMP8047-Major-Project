#include "FileSystem.h"

void FileSystem::WriteMazeDataToFile(std::string filename, std::string compact, std::string coded) {
	std::ofstream out("maze.txt");

	out << compact << std::endl;
	out << coded << std::endl;
	out.close();
}

std::pair<std::vector<std::vector<std::string>>, std::vector<std::vector<int>>> FileSystem::ReadMazeDataFile(std::string filename) {
	std::ifstream in(filename);
	std::string line;
	std::vector<std::string> lines;

	std::vector<std::vector<std::string>> stringRepMaze;
	std::vector<std::vector<int>> codedRepMaze;

	// every 1st line in a set is compact string, reconstruct it as string 2d vector
	// every 2nd second line is coded string, reconstruct it as int 2d vector

	while (std::getline(in, line)) {
		lines.push_back(line);
	}

	// TODO: handle a 64 char string as 8 x 8

	return std::make_pair(stringRepMaze, codedRepMaze);
}

void FileSystem::WriteModelToFile(std::string filename) {

}

void FileSystem::ReadModelFile(std::string filename) {

}