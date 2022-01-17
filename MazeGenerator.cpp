#include "MazeGenerator.h"

MazeGenerator::MazeGenerator(int rows, int cols) {
	height = rows;
	width = cols;

	mazeCells.resize(height);
	for (size_t i = 0; i < height; i++) {
		mazeCells[i].resize(width);
	}

}

// https://stackoverflow.com/questions/29739751/implementing-a-randomly-generated-maze-using-prims-algorithm
// https://github.com/keesiemeijer/maze-generator/blob/master/src/maze.js for wall removal

void MazeGenerator::Generate() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> xDistr(0, mazeCells.size()-1);
	std::uniform_int_distribution<> yDistr(0, mazeCells[0].size()-1);

	// Initialize a grid with all cells set as walls with no path
	for (int row = 0; row < mazeCells.size(); row++) {
		for (int col = 0; col < mazeCells[0].size(); col++) {
			mazeCells[row][col].SetWall(true);
		}
	}

	// Pick a cell to start a path
	int x = xDistr(gen);
	int y = yDistr(gen);
	std::cout << x << ", " << y << std::endl;
	mazeCells[x][y].SetWall(false); // Set new cell as path
	
	std::queue<MazeCell> frontierCells = ConvertToQueue(FrontierCellsOf(mazeCells[x][y]));

}

void MazeGenerator::WriteToFile() {

}

void MazeGenerator::PrintMaze() {
	if (width <= 1 || height <= 1) {
		return;
	}
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {

		}
	}

}

std::vector<std::vector<MazeCell>> MazeGenerator::GetMazeCells() {
	return mazeCells;
}

std::vector<MazeCell> MazeGenerator::FrontierCellsOf(MazeCell& cell) {
	return GetCellsAround(cell, true);
}

std::vector<MazeCell> MazeGenerator::PassageCellsOf(MazeCell& cell) {
	return GetCellsAround(cell, false);
}

std::vector<MazeCell> MazeGenerator::GetCellsAround(MazeCell& cell, bool isWall) {
	std::vector<MazeCell> frontier;
	for (int* dir : directions) {
		int newRow = cell.GetRow() + dir[0];
		int newCol = cell.GetColumn() + dir[1];
		if (IsValidPosition(newRow, newCol) && mazeCells[newRow][newCol].IsWall() == isWall) {
			frontier.push_back(mazeCells[newRow][newCol]);
		}
	}
	return frontier;
}

// Connects cells which are distance of 2 units apart
void MazeGenerator::ConnectCells(MazeCell& main, MazeCell& neighbour) {
	int betweenRow = (neighbour.GetRow() + main.GetRow()) / 2;
	int betweenCol = (neighbour.GetColumn() + main.GetColumn()) / 2;
	main.SetWall(false);
	mazeCells[betweenRow][betweenCol].SetWall(false);
	neighbour.SetWall(false);
}

bool MazeGenerator::IsValidPosition(int row, int col) {
	return row >= 0 && row < mazeCells.size() && col >= 0 && col < mazeCells[0].size();
}

std::queue<MazeCell> MazeGenerator::ConvertToQueue(std::vector<MazeCell> cells) {
	std::queue<MazeCell> cellQueue(std::deque<MazeCell>(cells.begin(), cells.end()));
	return cellQueue;
}
