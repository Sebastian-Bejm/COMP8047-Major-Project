#include "ObstructionGenerator.h"

ObstructionGenerator::ObstructionGenerator() {
	prevTime = clock();
}

void ObstructionGenerator::AttachMaze(std::vector<std::vector<MazeCell>> maze) {
	this->maze = maze;
}

void ObstructionGenerator::RunGenerator() {

}

void ObstructionGenerator::GenerateObstruction(ObjectTracker* tracker) {

}

bool ObstructionGenerator::IsValidLocation() {
	return false;
}

