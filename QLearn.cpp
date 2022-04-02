#include "QLearn.h"

QLearn::QLearn() {
	this->discountFactor = 0.0;
	this->epsilon = 0.0;
	this->epsDecayFactor = 0.0;
	this->learningRate = 0.0;
	this->numEpisodes = 1;
}

QLearn::QLearn(std::string filename, float discountFactor, float epsilon, float epsDecayFactor, float learningRate, int numEpisodes) {
	this->maze = FileSystem::ReadMazeDataFile(filename);

	this->discountFactor = discountFactor;
	this->epsilon = epsilon;
	this->epsDecayFactor = epsDecayFactor;
	this->learningRate = learningRate;
	this->numEpisodes = numEpisodes;

	EncodeMaze();
	CreateQTable();
}


void QLearn::Learn() {
	for (int i = 0; i < numEpisodes; i++) {

	}
}

void QLearn::EncodeMaze() {

	codedMaze.resize(maze.size());
	for (size_t i = 0; i < codedMaze.size(); i++) {
		codedMaze[i].resize(maze[i].size());
	}	

	for (size_t r = 0; r < maze.size(); r++) {
		for (size_t c = 0; c < maze[r].size(); c++) {
			codedMaze[r][c] = maze[r][c].GetCode();
		}
	}
}

void QLearn::CreateQTable() {

}

void QLearn::EpsilonGreedy() {

}