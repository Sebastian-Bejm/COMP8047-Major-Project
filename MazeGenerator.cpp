#include "MazeGenerator.h"

MazeGenerator::MazeGenerator(int rows, int cols) {
	height = rows;
	width = cols;

	mazeCells.resize(height);
	for (size_t i = 0; i < height; i++) {
		mazeCells[i].resize(width);
	}

	// Initialize all cells in the maze
	for (size_t row = 0; row < mazeCells.size(); row++) {
		for (size_t col = 0; col < mazeCells[row].size(); col++) {
			mazeCells[row][col] = MazeCell(row, col);
		}
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
	//std::cout << x << ", " << y << std::endl;
	mazeCells[x][y].SetAsStart(true);
	mazeCells[x][y].SetWall(false); // Set new cell as path
	
	std::deque<MazeCell> frontierCells = ConvertToDeque(FrontierCellsOf(mazeCells[x][y]));

	while (!frontierCells.empty()) {
		MazeCell& frontierCell = GetRandom(frontierCells);

		std::vector<MazeCell> frontierNeighbours = PassageCellsOf(frontierCell);

		if (!frontierNeighbours.empty()) {
			MazeCell& neighbour = GetRandom(frontierNeighbours);

			ConnectCells(frontierCell, neighbour);
		}

		std::vector<MazeCell> nextFrontiers = FrontierCellsOf(frontierCell);
		frontierCells.insert(frontierCells.end(), std::begin(nextFrontiers), std::end(nextFrontiers));

		frontierCells.erase(std::remove(frontierCells.begin(), frontierCells.end(), frontierCell), frontierCells.end());
	}

	PadOuterWalls();
}

void MazeGenerator::WriteToFile() {

}

void MazeGenerator::PrintMaze() {
	if (width <= 1 || height <= 1) {
		return;
	}
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			std::cout << mazeCells[row][col].str();
		}
		std::cout << std::endl;
	}

}

// Get the collection of maze cells
std::vector<std::vector<MazeCell>> MazeGenerator::GetMazeCells() {
	return mazeCells;
}

// Gets the frontier cells (wall cells) within a distance of 2 units
std::vector<MazeCell> MazeGenerator::FrontierCellsOf(MazeCell& cell) {
	return GetCellsAround(cell, true);
}

// Gets the passage cells within a distance of 2 units
std::vector<MazeCell> MazeGenerator::PassageCellsOf(MazeCell& cell) {
	return GetCellsAround(cell, false);
}

// Get the cells around a specific cell
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

	//main.SetWall(false);
	mazeCells[main.GetRow()][main.GetColumn()].SetWall(false);
	mazeCells[betweenRow][betweenCol].SetWall(false);
	//neighbour.SetWall(false);
	mazeCells[neighbour.GetRow()][neighbour.GetColumn()].SetWall(false);
}

bool MazeGenerator::IsValidPosition(int row, int col) {
	return row >= 0 && row < mazeCells.size() && col >= 0 && col < mazeCells[0].size();
}

MazeCell& MazeGenerator::GetRandom(std::deque<MazeCell>& cells) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> cellDistr(0, cells.size() - 1);

	int pos = cellDistr(gen);

	MazeCell& chosen = cells[pos];

	return chosen;
}

MazeCell& MazeGenerator::GetRandom(std::vector<MazeCell>& cells) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> cellDistr(0, cells.size() - 1);

	int pos = cellDistr(gen);

	MazeCell& chosen = cells[pos];
	
	return chosen;
}

void MazeGenerator::PadOuterWalls() {
	// Check the corners of the maze if there is already an outer wall in place
	// If there is no outer wall then recreate the maze with outer walls
	std::vector<std::vector<MazeCell>> paddedMaze;

	size_t rows = mazeCells.size(); // height
	size_t cols = mazeCells[0].size(); // width
	size_t newMazeWidth, newMazeHeight;
	std::cout << rows << " : " << cols << std::endl;

	// Check the top left, if there is a wall, check down and right
	if (mazeCells[0][0].IsWall()) {
		std::cout << "TL Wall" << std::endl;
		if (mazeCells[1][0].IsWall()) {
			//std::cout << "Top open" << std::endl;
			// Top wall is open, so pad north and south
			newMazeHeight = rows + 2;
		}
		if (mazeCells[0][1].IsWall()) {
			//std::cout << "Left open" << std::endl;
			// left wall is open, so pad east and west
			newMazeWidth = cols + 2;
		}
	}
	// There is no wall in the corner so both sections must be padded
	else {
		newMazeHeight = rows + 2;
		newMazeWidth = cols + 2;
	}
}

std::deque<MazeCell> MazeGenerator::ConvertToDeque(std::vector<MazeCell> cells) {
	std::deque<MazeCell> cellQueue(cells.begin(), cells.end());
	return cellQueue;
}
