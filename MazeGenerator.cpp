#include "MazeGenerator.h"

MazeGenerator::MazeGenerator() {}

// Initialize the maze
void MazeGenerator::InitMaze(int rows, int cols) {
	mazeCells.resize(rows);
	for (size_t i = 0; i < rows; i++) {
		mazeCells[i].resize(cols);
	}

	// Initialize all cells in the maze
	for (size_t row = 0; row < mazeCells.size(); row++) {
		for (size_t col = 0; col < mazeCells[row].size(); col++) {
			mazeCells[row][col] = MazeCell(row, col);
		}
	}
}

// Initializes an empty maze with walls
void MazeGenerator::InitWalledEmptyMaze(int rows, int cols) {
	mazeCells.resize(rows);
	for (size_t i = 0; i < rows; i++) {
		mazeCells[i].resize(cols);
	}

	for (size_t row = 0; row < mazeCells.size(); row++) {
		for (size_t col = 0; col < mazeCells[row].size(); col++) {
			mazeCells[row][col] = MazeCell(row, col);
			if (row == 0 || col == 0 || row == mazeCells.size()-1 || col == mazeCells.size()-1) {
				mazeCells[row][col].SetWall(true);
			}
		}
	}
}

// https://stackoverflow.com/questions/29739751/implementing-a-randomly-generated-maze-using-prims-algorithm
// Generates a random maze using Prim's algorithm
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
	int r = xDistr(gen);
	int c = yDistr(gen);

	mazeCells[r][c].SetWall(false); // Set new cell as path
	
	std::vector<MazeCell> frontierCells = FrontierCellsOf(mazeCells[r][c]);

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

	CreateMazePositions();

	EncodeMaze();

}

// Print the generated maze. Mainly used for debugging.
void MazeGenerator::PrintMaze() {
	if (mazeCells.size() <= 1 || mazeCells[0].size() <= 1) {
		return;
	}
	for (int row = 0; row < mazeCells.size(); row++) {
		for (int col = 0; col < mazeCells[0].size(); col++) {
			std::cout << mazeCells[row][col].str() << " ";
		}
		std::cout << std::endl;
	}

}

// Write the current maze to a file to be read on another run
void MazeGenerator::WriteMazeData() {
	std::string compactStr = "";
	std::string codedStr = "";
	std::ofstream out("maze.txt");

	for (size_t row = 0; row < mazeCells.size(); row++) {
		for (size_t col = 0; col < mazeCells[row].size(); col++) {
			compactStr += mazeCells[row][col].str();
			codedStr += std::to_string(encodedMaze[row][col]);
		}
	}

	out << compactStr << std::endl;
	out << codedStr << std::endl;
	out.close();
}

// Read the maze data from a file to be reconstructed and also used by the network
void MazeGenerator::ReadMazeData() {
	std::ifstream in("maze.txt");
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(in, line)) {
		lines.push_back(line);
	}

}

// Get the starting cell where the agent is first spawned in the maze
MazeCell& MazeGenerator::GetStartCell() {
	for (int row = 0; row < mazeCells.size(); row++) {
		for (int col = 0; col < mazeCells[0].size(); col++) {
			if (mazeCells[row][col].IsStart()) {
				return mazeCells[row][col];
			}
		}
	}
}

// Get the exit cell of the maze
MazeCell& MazeGenerator::GetEndCell() {
	for (int row = 0; row < mazeCells.size(); row++) {
		for (int col = 0; col < mazeCells[0].size(); col++) {
			if (mazeCells[row][col].IsExit()) {
				return mazeCells[row][col];
			}
		}
	}
}

// Get the starting point coordinates of the maze
std::vector<int> MazeGenerator::GetStartCoordinates() {
	std::vector<int> coords(2);
	for (int row = 0; row < mazeCells.size(); row++) {
		for (int col = 0; col < mazeCells[0].size(); col++) {
			if (mazeCells[row][col].IsStart()) {
				coords[0] = col;
				coords[1] = row;
			}
		}
	}
	return coords;
}

// Get the end point coordinates of the maze
std::vector<int> MazeGenerator::GetEndCoordinates() {
	std::vector<int> coords(2);
	for (int row = 0; row < mazeCells.size(); row++) {
		for (int col = 0; col < mazeCells[0].size(); col++) {
			if (mazeCells[row][col].IsExit()) {
				coords[0] = col;
				coords[1] = -row;
			}
		}
	}
	return coords;
}

// Get the collection of maze cells
std::vector<std::vector<MazeCell>>& MazeGenerator::GetMazeCells() {
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

	// Connect the cells by making them a path
	mazeCells[main.GetRow()][main.GetColumn()].SetWall(false);
	mazeCells[betweenRow][betweenCol].SetWall(false);
	mazeCells[neighbour.GetRow()][neighbour.GetColumn()].SetWall(false);
}

// Checks if a cell is in a valid position
bool MazeGenerator::IsValidPosition(int row, int col) {
	return row >= 0 && row < mazeCells.size() && col >= 0 && col < mazeCells[0].size();
}

// Get a random cell from the vector of maze cells
MazeCell& MazeGenerator::GetRandom(std::vector<MazeCell>& cells) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> cellDistr(0, cells.size() - 1);

	int pos = cellDistr(gen);

	MazeCell& chosen = cells[pos];
	return chosen;
}

// Encode the maze as 1's and 0's required for finding shortest path
void MazeGenerator::EncodeMaze() {
	encodedMaze.resize(mazeCells.size());
	for (size_t i = 0; i < mazeCells.size(); i++) {
		encodedMaze[i].resize(mazeCells[0].size());
	}

	for (size_t row = 0; row < mazeCells.size(); row++) {
		for (size_t col = 0; col < mazeCells[row].size(); col++) {
			if (mazeCells[row][col].IsWall()) {
				encodedMaze[row][col] = 1;
			}
			else {
				encodedMaze[row][col] = 0;
			}
		}
	}
}

// Helper function to create the outer walls if the generated maze does not have the correct walls
void MazeGenerator::PadOuterWalls() {
	// Check the corners of the maze if there is already an outer wall in place
	// If there are no outer walls then recreate the maze with outer walls
	std::vector<std::vector<MazeCell>> paddedMaze;

	const size_t rows = mazeCells.size(); // height
	const size_t cols = mazeCells[0].size(); // width

	size_t newMazeWidth = cols, newMazeHeight = rows;

	bool rowsAdded = false, colsAdded = false;

	// Check the top left, if there is a wall, check down and right
	if (mazeCells[0][0].IsWall()) {
		if (!mazeCells[0][1].IsWall()) {
			// Top/bottom wall is open, so pad north and south
			rowsAdded = true;
			newMazeHeight = rows + 2;
		}
		if (!mazeCells[1][0].IsWall()) {
			// Left/right wall is open, so pad east and west
			colsAdded = true;
			newMazeWidth = cols + 2;
		}
		// Both directions are properly walled, exit
		if (mazeCells[1][0].IsWall() && mazeCells[0][1].IsWall() ) {
			return;
		}
	}
	// There is no wall part in the corner so both directions must be padded
	else {
		rowsAdded = colsAdded = true;
		newMazeHeight = rows + 2;
		newMazeWidth = cols + 2;
	}

	// Resize the maze with the padding
	paddedMaze.resize(newMazeHeight);
	for (size_t i = 0; i < newMazeHeight; i++) {
		paddedMaze[i].resize(newMazeWidth);
	}

	// Check for rows or cols that have to be added
	size_t addRow = rowsAdded ? 1 : 0;
	size_t addCol = colsAdded ? 1 : 0;

	// Copy the maze to the padded maze
	for (int row = 0; row < paddedMaze.size(); row++) {
		for (int col = 0; col < paddedMaze[row].size(); col++) {
			if (row >= mazeCells.size() || col >= mazeCells[0].size()) {
				continue;
			}
			paddedMaze[row+addRow][col+addCol] = mazeCells[row][col];
		}
	}

	// If rows or columns were added set the walls accoringdly
	if (rowsAdded) {
		for (int i = 0; i < paddedMaze[0].size(); i++) {
			if (!paddedMaze[0][i].IsWall()) {
				paddedMaze[0][i].SetWall(true);
			}

			if (!paddedMaze[paddedMaze.size() - 1][i].IsWall()) {
				paddedMaze[paddedMaze.size() - 1][i].SetWall(true);
			}
		}
	}
	if (colsAdded) {
		for (int i = 0; i < paddedMaze.size(); i++) {
			if (!paddedMaze[i][0].IsWall()) {
				paddedMaze[i][0].SetWall(true);
			}
			
			if (!paddedMaze[i][paddedMaze[0].size() - 1].IsWall()) {
				paddedMaze[i][paddedMaze[0].size() - 1].SetWall(true);
			}
		}
	}

	// Set the padded maze to the original maze 
	mazeCells = paddedMaze;
}

// Sets the entrance and exit positions after correcting the maze
void MazeGenerator::CreateMazePositions() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> cellDistr(1, 4);

	const size_t rows = mazeCells.size();
	const size_t cols = mazeCells[0].size();
	//std::cout << "New - " << rows << " " << cols << std::endl;

	// Set the entrance point and exit points at opposite sides of the map
	int points[4][2] = {
		{1, 1}, // NW
		{1, cols - 2}, // NE
		{rows - 2, cols - 2}, // SE
		{rows - 2, 1}, // SW
	};

	int startPoint = cellDistr(gen);

	int startX = points[startPoint - 1][0];
	int startY = points[startPoint - 1][1];

	mazeCells[startX][startY].SetAsStart(true);

	// Set the end point diagonal to the start point
	int endPoint = 0;
	if (startPoint <= 2) {
		endPoint = startPoint + 2;
	}
	else if (startPoint > 2) {
		endPoint = startPoint - 2;
	}

	int endX = points[endPoint - 1][0];
	int endY = points[endPoint - 1][1];

	mazeCells[endX][endY].SetAsExit(true);
}