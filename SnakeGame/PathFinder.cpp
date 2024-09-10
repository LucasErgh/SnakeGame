#include "PathFinder.h"
#include <cmath>

void PathFinder::Delete() {
	delete snake;
}

PathFinder::PathFinder(int width, int height) : snake(NULL), width(width), height(height), max(width * height - 1), cycle(GenerateCycle()) {}

std::vector<Direction>* PathFinder::FindPath(Snake* newSnake, cords goal) {
	// Update Board
	this->goal = goal;
	UpdateSnake(newSnake);

	// Try to find the A* path
	std::vector<Direction>* path = FindAStarPath(goal);

	// Check if its safe
	if (safe(path))
	{
		return path;
	}
	// check if it can be made safe with rejoin function

	Snake* simulatedSnake = SimulateMove(path);
	if (simulatedSnake) {
		std::vector<Direction>* rejoinPath = RejoinCycle(simulatedSnake, width);
		if (rejoinPath) {
			delete simulatedSnake;
			simulatedSnake = NULL;
			// now append the old path to the new path
			int size = rejoinPath->size();
			for (int i = 0; i < size; ++i) {
				path->insert(path->begin(), rejoinPath->back());
				rejoinPath->pop_back();
			}
			delete rejoinPath;
			if (safe(path))
				return path;
		}
	}
	
	// Free unused memory
	if (simulatedSnake) {
		delete simulatedSnake;
		simulatedSnake = NULL;
	}
	delete path;
	path = NULL;

	// Now just try to rejoin cycle without new path
	std::vector<Direction>* cyclePath = FollowPath(1);
	return cyclePath;

}

void PathFinder::UpdateSnake(Snake* snake) {
	if (this->snake) {
		delete this->snake;
	}

	// Uses overloaded copy constructor to make a copy we can modify
	this->snake = new Snake(*snake);

	// Updates AStar's version of the snake
	UpdateAStar(this->snake);
}

bool PathFinder::safe(std::vector<Direction>* path) {	
	if (!path || path->empty()) return false;

		// Get the snake after the move so we can test for saftey
		Snake* temp;
		temp = SimulateMove(path);
		
		if (!temp) return false;

		// Now check if we can travel on the cycle for a number of moves that
		// grows exponentially based on the size of the snake
		int turns = temp->getSize() * 1.5;// std::max(12.0, std::pow(2, snake->size / 10));
	
		for (int i = 0; i < turns; ++i) {
			if (!temp->isAlive() || IsOposite(temp->getDirection(), nextDir(temp->headLocation())))
				return false;
			temp->changeDirection(nextDir(temp->headLocation()));
			temp->move();
			if (!temp->isAlive() || IsOposite(temp->getDirection(), nextDir(temp->headLocation())))
				return false;
		}
		
		delete temp;

		return true;
}

int PathFinder::distance(cords a, cords b) {
	int distance = 0;
	if (a.first > b.first)	distance += a.first - b.first;
	else					distance += b.first - a.first;
	if (a.second > b.second)distance += a.second - b.second;
	else					distance += b.second - a.second;
	return distance;
}

std::vector<Direction>* PathFinder::RejoinCycle(Snake* snake, int radius) {

	// otherwise we find a path back to the cycle
	std::vector<cords> nodes;
	int xmin, ymin, xmax, ymax;
	xmin = (snake->headLocation().first - radius >= 1) ? snake->headLocation().first - radius : 1;
	ymin = (snake->headLocation().second - radius >= 1) ? snake->headLocation().second - radius : 1;
	xmax = (snake->headLocation().first + radius <= width) ? snake->headLocation().first + radius : width;
	ymax = (snake->headLocation().second + radius <= height) ? snake->headLocation().first + radius : height;

	// load list of all ordered pairs into nodes
	for (int i = ymin; i <= ymax; ++i) {
		for (int j = xmin; j <= xmax; ++j) {
			nodes.push_back(std::make_pair(j, i));
		}
	}

	bool safepath = false;
	int minDistance = height * width, tempd;

	std::vector<Direction>* path = NULL;

	while (!nodes.empty()) {
		
		cords closestNode;
		int closestNodeIndex = -1;

		// Find closest node to snake
		for (int i = 0; i < nodes.size(); ++i) {
			cords cur = nodes[i];
			int currentDistance = distance(cur, snake->headLocation());
			if (currentDistance < minDistance) {
				minDistance = distance(cur, snake->headLocation());
				closestNode = cur;
				closestNodeIndex = i;
			}
		}

		// Erase the closest node from the list
		if (closestNodeIndex != -1) {
			nodes.erase(nodes.begin() + closestNodeIndex);
		}

		// Try to find a path to the closest node
		UpdateAStar(snake);
		path = FindAStarPath(closestNode);
		if (path && safe(path)) {
			return path;  // Return the first safe path found
		}
		else {
			delete path;  // Clean up the memory if the path is unsafe
			path = nullptr;  // Reset the pointer for the next iteration
		}

		// Reset minDistance for the next iteration
		minDistance = height * width;
	}
	return NULL;
}

Snake* PathFinder::SimulateMove(std::vector<Direction>* path) {
	// Make copy of snake and path as not to damage either
	Snake* temp = new Snake(*snake);
	std::vector<Direction>* tempPath = new std::vector<Direction>(*path);

	// Now move through
	Direction dir;
	while (!tempPath->empty()) {
		dir = tempPath->back();
		tempPath->pop_back();
		temp->changeDirection(dir);
		temp->move();
		if (!temp->isAlive()) {
			delete temp;
			delete tempPath;
			return NULL;
		}
	}
	delete tempPath;
	return temp;
}

std::vector<Direction>* PathFinder::FollowPath(int num) {
	cords cur = snake->headLocation();
	std::vector<Direction>* path = new std::vector<Direction>;
	for (int i = 0; i < num; ++i) {
		path->insert(path->begin(), nextDir(cur));
		Shift((*path)[0], cur.second, cur.first);
	}
	return path;
}

Direction PathFinder::prevDir(cords cords) {
	int x = cords.first - 1, y = cords.second - 1;
	int next = (cycle[y][x] == 0) ? max : cycle[y][x] - 1;

	if (y < height - 1 && cycle[y + 1][x] == next) return down;
	else if (x > 0 && cycle[y][x - 1] == next) return left;
	else if (y > 0 && cycle[y - 1][x] == next) return up;
	else if (x < width - 1 && cycle[y][x + 1] == next) return right;
	else return none;
}

Direction PathFinder::nextDir(cords cords) {
	int x = cords.first - 1, y = cords.second - 1;
	int next = (cycle[y][x] == max) ? 0 : cycle[y][x] + 1;

	if (y < height - 1 && cycle[y + 1][x] == next) return down;
	else if (x > 0 && cycle[y][x - 1] == next) return left;
	else if (y > 0 && cycle[y - 1][x] == next) return up;
	else if (x < width - 1 && cycle[y][x + 1] == next) return right;
	else return none;
}

void PathFinder::Shift(Direction dir, int& y, int& x) {
	switch (dir)
	{
	case up: --y; return;
	case down: ++y; return;
	case left: --x; return;
	case right: ++x; return;
	default: throw(1);
	}
}

bool PathFinder::GetDirection(int** grid, int y, int x, Direction* dir1, Direction* dir2, int height, int width, int max, int min) {
	int x2, y2;
	int next = (grid[y][x] == max) ? min : grid[y][x] + 1;
	int previous = (grid[y][x] == min) ? max : grid[y][x] - 1;

	if (y < height - 1 && grid[y + 1][x] == next) *dir2 = down;
	else if (x > 0 && grid[y][x - 1] == next) *dir2 = left;
	else if (y > 0 && grid[y - 1][x] == next) *dir2 = up;
	else if (x < width - 1 && grid[y][x + 1] == next) *dir2 = right;
	else return false;

	if (dir1 == NULL) return true;
	if (y < height - 1 && grid[y + 1][x] == previous) *dir1 = up;
	else if (x > 0 && grid[y][x - 1] == previous) *dir1 = right;
	else if (y > 0 && grid[y - 1][x] == previous) *dir1 = down;
	else if (x < width - 1 && grid[y][x + 1] == previous) *dir1 = left;
	else return false;

	return true;
}

int** PathFinder::GenerateCycle() {
	// At the moment this only works for square grids with side lengths divisible by four
	if (width != height || width % 2 != 0) {
		throw(1);
	}

	// The method I'm using for creating the cycles was taken from 
	// a paper called "Turns in Hamilton cycles of rectangular grids" 
	// written by Ethan Y. Tan and Guowen Zhang

	// We start by making a cycle with minimum number of turns for a grid of half its length and width
	int minLength = width / 2;
	int** minimum = new int* [minLength];
	for (int i = 0; i < minLength; ++i) {
		minimum[i] = new int[minLength];
	}

	// Now we create the "pronged" cycle with half side lengths
	int cur = 0;
	minimum[0][0] = cur++;
	for (int i = 0; i < minLength; i += 2) {
		for (int j = 1; j < minLength; ++j) {
			minimum[i][j] = cur++;
		}
		for (int k = minLength - 1; k > 0; --k) {
			minimum[i + 1][k] = cur++;
		}
	}
	for (int i = minLength - 1; i > 0; --i) {
		minimum[i][0] = cur++;
	}

	// Now we must dialate the cycle from (0, 0) by a factor of 2
	// with two added rules. For each turn reduce its x-cordinate
	// by 1 if its vertical segment is oriented up, and reduce its
	// y-cordinate by 1 if its horizontal segment is oriented right.
	// We must also rotate it clockwise before we shift segments.

	// allocate memory for the dilated cycle
	int** minDilated;
	int DilatedMax;
	int length = width;
	minDilated = new int* [length];
	for (int i = 0; i < length; ++i) {
		minDilated[i] = new int[length];
		for (int j = 0; j < length; ++j) {
			minDilated[i][j] = -1; // initialize to -1 for debugging
		}
	}

	// Variables to keep track of current position and direction
	Direction enter, exit = none;
	int Ox1 = 0, Oy1 = 0, Ox2 = 0, Oy2 = 0;
	int Nx1, Ny1, Nx2, Ny2;
	int number = 0;
	bool running = true;
	int val = 0;
	int startx, starty;

	// Find starting point
	if (minimum[0][0] != 0) {
		for (int j = 0; j < minLength; ++j) {
			for (int k = 0; k < minLength; ++k) {
				if (minimum[j][k] == val) {
					Oy1 = j;
					Ox1 = k;
					break;
				}
			}
		}
	}

	// Find starting point
	if (Oy1 != minLength - 1 && minimum[Oy1 + 1][Ox1] == minLength * minLength - 1) enter = up;
	else if (Ox1 != minLength - 1 && minimum[Oy1][Ox1 + 1] == minLength * minLength - 1) enter = left;
	else if (Oy1 != 0 && minimum[Oy1 - 1][Ox1] == minLength * minLength - 1) enter = down;
	else if (Ox1 != 0 && minimum[Oy1][Ox1 - 1] == minLength * minLength - 1) enter = left;
	else throw(1);

	Ox2 = Ox1;
	Oy2 = Oy1;

	while (running) {
		// Loop to find the next turn
		do {
			int next = (val >= minLength * minLength - 1) ? val - (minLength * minLength - 1) : val + 1;

			if (!GetDirection(minimum, Oy2, Ox2, &enter, &exit, minLength, minLength, minLength * minLength - 1)) {
				throw(1); // If enterance isn't correct throw
			}

			// move to the next cell to check for turn
			Shift(exit, Oy2, Ox2);

			++val;
			if (exit == none) {
				throw(1);
			}
		} while (enter == exit);

		Nx2 = Ox2;
		Ny2 = Oy2;

		// undo last move
		Shift(Oposite(exit), Ny2, Nx2);

		// scale old points to new points
		Nx2 = Nx2 * 2 + 1;
		Ny2 = Ny2 * 2 + 1;

		// shift points if they traveled right or down
		if (enter == right || exit == right) --Ny2;
		if (exit == down || enter == down) --Nx2;

		if (number == 0) {
			Ny1 = Ny2;
			Nx1 = Nx2;
			startx = Nx1;
			starty = Ny1;
			// this will set the both corners to the same value skiping the next loop
			minDilated[Ny1][Nx1] = number++;
		}

		// fill in points between corners
		while ((Nx1 != Nx2 || Ny1 != Ny2)) {

			Shift(enter, Ny1, Nx1);
			if (Nx1 == startx && Ny1 == starty)
				break;
			minDilated[Ny1][Nx1] = number++;
		}

		// Reset variables
		Ox1 = Ox2;
		Oy1 = Oy2;
		Ny1 = Ny2;
		Nx1 = Nx2;
		enter = exit;
		exit = none;

		// Check if we have completed the cycle
		if (number > 2 && Ny2 == starty && Nx2 == startx) {
			DilatedMax = number - 1;
			break;
		}
	}

	// Cleam memory
	for (int i = 0; i < minLength; ++i) {
		delete[] minimum[i];
	}
	delete[] minimum;

	/*
	Now that we have the scaled minimum turns of half the side length grid
	we now start making the maximum turns version by creating a grid of closed
	loops. Then we find the reduced form, which has maximum number of turns.
	*/

	// Initialize both new grids we will need
	int** SquareCycles = new int* [height];
	int** ReducedGrid = new int* [height];

	for (int i = 0; i < height; ++i) {
		SquareCycles[i] = new int[width];
		ReducedGrid[i] = new int[width];
		for (int j = 0; j < width; ++j) {
			// initialize each cell in new grids to -1
			SquareCycles[i][j] = -1;
			ReducedGrid[i][j] = -1;
		}
	}

	// Fill SquareCycles Grid
	number = 0;
	for (int j = 0; j < height; j += 2) {
		for (int k = 0; k < width; k += 2) {
			SquareCycles[j][k] = number++;
			SquareCycles[j][k + 1] = number++;
			SquareCycles[j + 1][k + 1] = number++;
			SquareCycles[j + 1][k] = number++;
		}
	}

	// Find the union of SquareCycles Grid and minDialated then put it in ReducedGrid
	// (I beleive its actually not the union, but rather, the difference of their union and their intersection but Ill be saying union)
	Direction SquareEnter = none, SquareExit = none, DialatedEnter = none, DialatedExit = none, UnionEnter = up, UnionExit = none;
	int y1 = 0, x1 = 0, // these keep track of our position in the ReducedGrid
		y2 = 0, x2 = 0; // these keep track of the next turn in the ReducedGrid
	int curUnion = 0;


	// Find next cells 
	while (curUnion < height * width) {
		if (curUnion == height * width) {
			throw(1);
		}

		// This loop find the next turn
		do {
			// We need to find the min and max of the small cycle to get the direction in the SquareGrid
			int tempx, tempy;

			if (x2 % 2 == 0) tempx = x2;
			else			 tempx = x2 - 1;

			if (y2 % 2 == 0) tempy = y2;
			else			 tempy = y2 - 1;

			GetDirection(SquareCycles, y2, x2, &SquareEnter, &SquareExit, height, width, SquareCycles[tempy + 1][tempx], SquareCycles[tempy][tempx]);

			// switch direction of enterances
			Direction OpositeEnter = Oposite(UnionEnter);
			SquareEnter = Oposite(SquareEnter);

			// if the dialated grid isn't filled in here we just go with the square grid
			if (minDilated[y2][x2] == -1) {
				if (OpositeEnter != SquareExit)
					UnionExit = SquareExit;
				else
					UnionExit = SquareEnter;
			}
			// otherwise we have to find the union
			else {
				// now find the direction of the current cel in minDialated and SquareCycles
				if (!GetDirection(minDilated, y2, x2, &DialatedEnter, &DialatedExit, height, width, DilatedMax)) {
					throw(1);
				}

				// swap the enterances because those are in the oposite direction
				DialatedEnter = Oposite(DialatedEnter);

				// Now we find the correct direction
				if (DialatedEnter != none && DialatedEnter != SquareEnter && DialatedEnter != SquareExit && DialatedEnter != OpositeEnter) {
					UnionExit = DialatedEnter;
				}
				else if (DialatedExit != none && DialatedExit != SquareEnter && DialatedExit != SquareExit && DialatedExit != OpositeEnter) {
					UnionExit = DialatedExit;
				}
				else if (SquareEnter != none && SquareEnter != DialatedEnter && SquareEnter != DialatedExit && SquareEnter != OpositeEnter) {
					UnionExit = SquareEnter;
				}
				else if (SquareExit != none && SquareExit != DialatedEnter && SquareExit != DialatedExit && SquareExit != OpositeEnter) {
					UnionExit = SquareExit;
				}
				else throw(1);
			}

			// move to next cell to check for turn
			Shift(UnionExit, y2, x2);

		} while (UnionEnter == UnionExit);

		int BSy = y2, BSx = x2; // positions before they were shifted back

		// undo last move
		Shift(Oposite(UnionExit), y2, x2);

		// Mark points in ReducedGrid
		if (curUnion == 0) {
			// this will set the both corners to the same value skiping the next loop
			ReducedGrid[y1][x1] = curUnion++;
			x1 = x2;
			y1 = y2;
		}
		else {
			// fill in points between corners
			while ((x1 != x2 || y1 != y2)) {
				if (x1 == 0 && y1 == 0)
					break;
				ReducedGrid[y1][x1] = curUnion++;
				Shift(UnionEnter, y1, x1);
			}
			// fill corner
			if (x1 == 0 && y1 == 0)
				break;
			ReducedGrid[y2][x2] = curUnion++;
		}

		// set starting points to new corner
		x2 = BSx;
		y2 = BSy;
		x1 = x2;
		y1 = y2;
		UnionEnter = UnionExit;
		UnionExit = none;
	}
	for (int i = 0; i < height; ++i) {
		delete[] SquareCycles[i];
		delete[] minDilated[i];
	}
	delete[] minDilated;
	delete[] SquareCycles;
	return ReducedGrid;
}

Node* PathFinder::findNode(NodeSet& nodes, cords cur) {
	for (auto node : nodes) {
		if (node->pos == cur) {
			return node;
		}
	}
	return NULL;
}

std::vector<Direction>* PathFinder::getDirections(CordList cordList) {
	std::vector<Direction>* directions = new std::vector<Direction>;

	for (int i = 0; i < cordList.size() - 1; ++i) {
		cords cur = cordList[i];
		cords next = cordList[i + 1];

		if (cur.second > next.second) directions->push_back(down);
		else if (cur.second < next.second) directions->push_back(up);
		else if (cur.first < next.first) directions->push_back(left);
		else if (cur.first > next.first) directions->push_back(right);
	}
	return directions;
}

void PathFinder::UpdateAStar(Snake* snake) {
	walls.reserve(1);
	walls = snake->bodyLocation();

	start = snake->headLocation();
	startDir = snake->getDirection();
	height = snake->height;
	width = snake->width;
	size = snake->getSize();
}

std::vector<Direction>* PathFinder::FindAStarPath(cords goal) {
	Node* current = NULL;
	NodeSet openSet; // This is for nodes who have not been checked
	NodeSet	closedSet; // This is for nodes who have been checked

	openSet.push_back(new Node(start, NULL, startDir));

	while (!openSet.empty()) {
		auto root = openSet.begin();
		current = *root;

		// Find best node as of now
		for (auto cur = openSet.begin(); cur != openSet.end(); ++cur) {
			auto node = *cur;
			if (node->GetScore() <= current->GetScore()) {
				current = node;
				root = cur;
			}
		}

		// Check if path was found
		if (current->pos == goal) {
			break;
		}

		//  
		closedSet.push_back(current);
		openSet.erase(root);

		// Grow graph from new best cell
		for (int i = 0; i < 4; ++i) {
			// make new cell in correct direction
			if (IsOposite((Direction)i, current->dir)) continue;

			cords newCords = current->pos;
			Shift((Direction)i, newCords);
			if (collision(newCords, current->G + 1) || findNode(closedSet, newCords)) {
				continue;
			}

			int totalCost = current->G + 1;

			Node* successor = findNode(openSet, newCords);
			if (!successor) {
				successor = new Node(newCords, current, (Direction)i);
				successor->G = totalCost;
				successor->H = distance(successor->pos, goal);
				openSet.push_back(successor);
			}
			else if (totalCost < successor->G) {
				successor->parent = current;
				successor->G = totalCost;
			}
		}
	}

	CordList path;
	while (current != NULL) {
		path.push_back(current->pos);
		current = current->parent;
	}

	for (auto cur : openSet) {
		delete cur;
	}
	for (auto cur : closedSet) {
		delete cur;
	}

	return getDirections(path);
}

bool PathFinder::collision(cords check, int movement) {
	if (check.first < 1 || check.first > width || check.second < 1 || check.second > height)
		return true;
	for (int i = 0; i < walls.size(); ++i) {
		if (walls[i] == check && (size - i) > movement) {
			return true;
		}
	}


	return false;
}

void PathFinder::Shift(Direction dir, cords& cur) {
	switch (dir)
	{
	case up: --cur.second; return;
	case down: ++cur.second; return;
	case left: --cur.first; return;
	case right: ++cur.first; return;
	default: throw(1);
	}
}

Node::Node(cords cords, Node* parent, Direction direction) {
	pos = cords;
	this->parent = parent;
	G = H = 0;
	dir = direction;
}

int Node::GetScore()
{
	return G + H;
}