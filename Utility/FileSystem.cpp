#include "FileSystem.h"

// Writes the current maze structure to a file
void FileSystem::WriteMazeDataToFile(std::vector<std::vector<MazeCell>> mazeCells) {
	std::string compactStr = "";
	std::ofstream out("maze.txt");

	for (size_t row = 0; row < mazeCells.size(); row++) {
		for (size_t col = 0; col < mazeCells[row].size(); col++) {
			compactStr += mazeCells[row][col].str();
		}
		compactStr += "\n";
	}

	out << compactStr;
	out.close();
}

// Read maze data structure from a file and reconstruct it for rendering
std::vector<std::vector<MazeCell>> FileSystem::ReadMazeDataFile(std::string filename) {
	std::ifstream in(filename);
	std::string templine;
	std::vector<std::string> lines;

	while (std::getline(in, templine)) {
		lines.push_back(templine);
	}

	std::vector<std::vector<MazeCell>> fullMaze;
	fullMaze.resize(lines.size());
	for (size_t i = 0; i < lines.size(); i++) {
		fullMaze[i].resize(lines[i].size());
	}

	for (size_t r = 0; r < lines.size(); r++) {
		for (size_t c = 0; c < lines[r].size(); c++) {
			if (lines[r][c] == '#') {
				fullMaze[r][c].SetWall(true);
			}
			else if (lines[r][c] == '.') {
				fullMaze[r][c].SetWall(false);
			}
			else if (lines[r][c] == 'S') {
				fullMaze[r][c].SetAsStart(true);
			}
			else if (lines[r][c] == 'E') {
				fullMaze[r][c].SetAsExit(true);
			}
		}
	}

	return fullMaze;
}

// Writes a trained model to a file
void FileSystem::WriteModelToFile(std::string filename) {

}

// Read a model file and reconstruct the trained model
void FileSystem::ReadModelFile(std::string filename) {

}