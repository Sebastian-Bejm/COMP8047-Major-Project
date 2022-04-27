#include "MazeGenerator.h"

// Returns the singleton instance of this class
MazeGenerator& MazeGenerator::GetInstance() {
	static MazeGenerator instance;
	return instance;
}

// Initialize the maze
void MazeGenerator::InitMaze(int rows, int cols, int wallsToRemove) {
	mazeCells.resize(rows);
	for (size_t i = 0; i < rows; i++) {
		mazeCells[i].resize(cols);
	}

	// Initialize all cells in the maze
	for (size_t row = 0; row < mazeCells.size(); row++) {
		for (size_t col = 0; col < mazeCells[row].size(); col++) {
			mazeCells[row][col] = MazeCell(row, col, true);
		}
	}

	int maxMazeSize = rows * cols;
	wallRemovalLimit = (int)(0.8 * maxMazeSize);

	if (wallsToRemove >= wallRemovalLimit) {
		this->wallsToRemove = wallRemovalLimit;
	}
	else {
		this->wallsToRemove = wallsToRemove;
	}
}

// https://stackoverflow.com/questions/29739751/implementing-a-randomly-generated-maze-using-prims-algorithm
// Generates a random maze using Prim's algorithm
void MazeGenerator::Generate() {
	const int rowSize = static_cast<int>(mazeCells.size() - 1);
	const int colSize = static_cast<int>(mazeCells[0].size() - 1);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> xDistr(0, rowSize);
	std::uniform_int_distribution<> yDistr(0, colSize);

	// Pick a cell to start a path
	int r = xDistr(gen);
	int c = yDistr(gen);

	mazeCells[r][c].SetWall(false); // Set new cell as path
	
	std::vector<MazeCell> frontierCells = FrontierCellsOf(mazeCells[r][c]);

	// If we have path cells not open, continue
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

	// Pad the outer walls if we don't have correct walls
	PadOuterWalls(); 

	// Set the start and end points
	CreateMazePositions();

	// Start trying to remove walls
	RemoveWalls();

	// If there are locations where there are wall "islands" add more obstructions
	MarkObstructions();
}

// Print the generated maze. Mainly used for debugging.
void MazeGenerator::PrintMaze() {
	if (mazeCells.size() <= 1 || mazeCells[0].size() <= 1) {
		return;
	}
	for (size_t row = 0; row < mazeCells.size(); row++) {
		for (size_t col = 0; col < mazeCells[0].size(); col++) {
			std::cout << mazeCells[row][col].str() << " ";
		}
		std::cout << std::endl;
	}

}

// Get the starting cell where the agent is first spawned in the maze
MazeCell& MazeGenerator::GetStartCell() {
	for (size_t row = 0; row < mazeCells.size(); row++) {
		for (size_t col = 0; col < mazeCells[0].size(); col++) {
			if (mazeCells[row][col].IsStart()) {
				return mazeCells[row][col];
			}
		}
	}
}

// Get the exit cell of the maze
MazeCell& MazeGenerator::GetEndCell() {
	for (size_t row = 0; row < mazeCells.size(); row++) {
		for (size_t col = 0; col < mazeCells[0].size(); col++) {
			if (mazeCells[row][col].IsExit()) {
				return mazeCells[row][col];
			}
		}
	}
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
	const int size = static_cast<int>(cells.size() - 1);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> cellDistr(0, size);

	int pos = cellDistr(gen);

	MazeCell& chosen = cells[pos];
	return chosen;
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
	for (size_t row = 0; row < paddedMaze.size(); row++) {
		for (size_t col = 0; col < paddedMaze[row].size(); col++) {
			if (row >= mazeCells.size() || col >= mazeCells[0].size()) {
				continue;
			}
			paddedMaze[row+addRow][col+addCol] = mazeCells[row][col];
			// Shift cells over when additional rows or columns are added
			paddedMaze[row + addRow][col + addCol].SetNewPos(row + addRow, col + addCol);
		}
	}

	// If rows were added set the new cells as walls
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
	// If columns were added set the new cells as walls
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

	// Set the entrance point and exit points at opposite sides of the map
	int points[4][2] = {
		{1, 1}, // NW
		{1, cols - 2}, // NE
		{rows - 2, cols - 2}, // SE
		{rows - 2, 1}, // SW
	};

	// Choose a random start point from one of the 4 corners of the maze
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

// Remove walls from the current maze and mark them as obstructions for later
void MazeGenerator::RemoveWalls() {
	if (wallsToRemove == 0) {
		return;
	}

	const int maxTries = 300;
	const int min = 1;
	const int maxRow = mazeCells.size() - 1;
	const int maxCol = mazeCells[0].size() - 1;

	int tries = 0;
	int wallsRemoved = 0;

	std::random_device rd; 
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> distr(min, maxRow);

	while (tries < maxTries) {
		tries++;

		// If we reached the goal, exit
		if (wallsRemoved >= wallsToRemove) {
			break;
		}

		// Get random row from maze
		//int y = min + rand() % ((maxRow + 1) - min);
		int y = distr(gen);
		y = (y == maxRow) ? y - 1 : y;

		std::vector<int> walls; 
		std::vector<MazeCell> row = mazeCells[y];

		// Get walls from random row
		for (size_t i = 0; i < row.size(); i++) {
			if (i == 0 || i == row.size() - 1) {
				continue;
			}
			if (mazeCells[y][i].IsWall()) {
				walls.push_back((int)i);
			}
		}

		// Shuffle the vector randomly
		std::random_shuffle(walls.begin(), walls.end());

		// Try breaking a wall for this row
		for (size_t i = 0; i < walls.size(); i++) {
			if (RemoveWall(y, walls[i])) {
				// Wall can be broken
				wallsRemoved++;
				break;
			}
			else if (walls.size() == 1) {
				RemoveWall(y, walls[0]);
			}
		}
	}
}

// Checks if a wall can be removed at (row, col)
// When a wall meets the criteria for removal, mark it as obstruction
bool MazeGenerator::RemoveWall(int row, int col) {
	// Remove wall if possible
	bool evenRow = (row % 2 == 0);
	bool evenCol = (col % 2 == 0);

	MazeCell cell = mazeCells[row][col];

	// If the cell isn't a wall (normal path) then immediately return
	if (!cell.IsWall()) {
		return false;
	}

	if (!evenRow && evenCol) {
		// Uneven row and even column
		bool hasTop = (row - 2 > 0) && (mazeCells[row - 2][col].IsWall());
		bool hasBottom = (row + 2 < mazeCells.size()) && (mazeCells[row + 2][col].IsWall());

		if (hasTop && hasBottom) {
			mazeCells[row][col].SetObstruction(true);
			return true;
		}
		else if (!hasTop && hasBottom) {
			bool left = mazeCells[row - 1][col - 1].IsWall();
			bool right = mazeCells[row - 1][col + 1].IsWall();
			if (left || right) {
				mazeCells[row][col].SetObstruction(true);
				return true;
			}
		}
		else if (hasTop && !hasBottom) {
		bool left = mazeCells[row + 1][col - 1].IsWall();
		bool right = mazeCells[row + 1][col + 1].IsWall();
		if (left || right) {
			mazeCells[row][col].SetObstruction(true);
			return true;
		}
		}

	}
	else if (evenRow && !evenCol) {
	// Even row and uneven column
	bool hasLeft = mazeCells[row][col - 2].IsWall();
	bool hasRight = mazeCells[row][col + 2].IsWall();

	if (hasLeft && hasRight) {
		mazeCells[row][col].SetObstruction(true);
		return true;
	}
	else if (!hasLeft && hasRight) {
		bool top = mazeCells[row - 1][col - 1].IsWall();
		bool bottom = mazeCells[row + 1][col - 1].IsWall();
		if (top || bottom) {
			mazeCells[row][col].SetObstruction(true);
			return true;
		}
	}
	else if (hasLeft && !hasRight) {
		bool top = mazeCells[row - 1][col + 1].IsWall();
		bool bottom = mazeCells[row + 1][col + 1].IsWall();
		if (top || bottom) {
			mazeCells[row][col].SetObstruction(true);
			return true;
		}
	}
	}

	return false;
}

// Mark additional obstructions in available locations where the wall removal hadn't marked them
void MazeGenerator::MarkObstructions() {
	// mark obstacles where there are "wall islands"
	// . . .
	// . # .
	// . . .

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(1, 4);

	for (size_t i = 0; i < mazeCells.size(); i++) {
		for (size_t j = 0; j < mazeCells[i].size(); j++) {
			MazeCell currentCell = mazeCells[i][j];
			// check index so we dont get an out of bounds error
			if (currentCell.IsWall() && i > 1 && j > 1 && i < mazeCells.size() - 1 && j < mazeCells.size() - 1) {

				// check if there are surrounding walls in the four directions
				if (!mazeCells[i - 1][j].IsWall() && !mazeCells[i + 1][j].IsWall()
					&& !mazeCells[i][j - 1].IsWall() && !mazeCells[i][j + 1].IsWall()) {

					// generate a random spot for obstruction
					// W = 1, E = 2, N = 3, S = 4
					int loc = distr(gen);
					int x = 0, y = 0;
					switch (loc) {
					case 1:
						x = -1;
						break;
					case 2:
						x = 1;
						break;
					case 3:
						y = 1;
						break;
					case 4:
						y = -1;
						break;
					}

					mazeCells[i + y][j + x].SetObstruction(true);
				}

			}
		}
	}

	// remove obstructions near end point to resolve crash issue
	MazeCell end = GetEndCell();
	size_t r = end.GetRow();
	size_t c = end.GetColumn();

	if (mazeCells[r - 1][c].IsObstruction()) {
		mazeCells[r - 1][c].SetObstruction(false);
	}
	else if (mazeCells[r + 1][c].IsObstruction()) {
		mazeCells[r + 1][c].SetObstruction(false);
	}
	else if (mazeCells[r][c - 1].IsObstruction()) {
		mazeCells[r][c - 1].SetObstruction(false);
	}
	else if (mazeCells[r][c + 1].IsObstruction()) {
		mazeCells[r][c + 1].SetObstruction(false);
	}
}
