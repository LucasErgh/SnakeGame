#include "PathFinder.h"
#include <cmath>

void PathFinder::Delete() {
	snake->deleteNodes();
	delete snake;
}

PathFinder::PathFinder(int width, int height) : cycle(HamCycle(width, height)), star(AStar(snake)), snake(NULL), width(width), height(height), max(width * height - 1) {}

std::vector<Direction>* PathFinder::FindPath(Snake* newSnake, cords goal) {
	// Update Board
	this->goal = goal;
	UpdateSnake(newSnake);
	
	static bool tryShortCut = true;
	// after the snake has filled 80% of the board we no longer try to shortcut because
	// it is likely that apples will show up unexpectedly preventing the snake from following the cycle
	if (tryShortCut && snake->size > width * height * .8) tryShortCut = false;
	
	std::vector<Direction>* path;
	if (tryShortCut) {
		path = star.FindPath(goal);
	}
	else {
		return RejoinCycle(max);
	}
	
	// Now we check to see if the A* path is safe
	std::vector<Direction>* rejoinPath = RejoinCycle(max / 2);
	
	// If so we use that
	if (rejoinPath) {
		while(!rejoinPath->empty()){
			path->insert(path->begin(), rejoinPath->back());
			rejoinPath->pop_back();
		}
	
	}
	// Otherwise we follow the cycle
	else {
		delete path;
		path = RejoinCycle(max);
	}
	delete rejoinPath;
	return path;	
}

void PathFinder::UpdateSnake(Snake* snake) {
	if (this->snake) {
		this->snake->deleteNodes();
		delete this->snake;
	}

	// Uses overloaded copy constructor to make a copy we can modify
	this->snake = new Snake(*snake);

	// Updates AStar's version of the snake
	star.updateSnake(this->snake);
}

bool PathFinder::safe(std::vector<Direction>* path) {	
		// Get the snake after the move so we can test for saftey
		Snake temp = *SimulateMove(path);

		// Now check if we can travel on the cycle for a number of moves that
		// grows exponentially based on the size of the snake
		int turns = temp.size + 3;
		turns = (turns > height * width - 1) ? height * width - 1 : turns;
	
		for (int i = 0; i < turns; ++i) {
			if (temp.died() || IsOposite(temp.direction, cycle.nextDir(temp.headLocation()))) 
				return false;
			temp.changeDirection(cycle.nextDir(temp.headLocation()));
			temp.move();
		}
		return true;
}

std::vector<Direction>* PathFinder::RejoinCycle(int attempts) {
	int cur = 0;
	// Find nearest node to rejoin
	cords toJoin = nextCycleNode(snake->headLocation());
	// Check if it can contine, if not loop to the next spot and check that
	std::vector<Direction> path = *star.FindPath(toJoin);
	while (!safe(&path) && cur <= attempts) {
		toJoin = nextCycleNode(toJoin);
		path = *star.FindPath(toJoin);
		if (toJoin == snake->headLocation()) return NULL;
		++cur;
	}
	// Join
	return &path;
}

cords PathFinder::nextCycleNode(cords start) {
	int max = height * width - 1;
	int next = cycle[start.second][start.first];
	
	next = (next == max) ? 0 : next + 1;

	if (cycle[start.second + 1][start.first] == next) return std::make_pair(start.second + 1, start.first);
	else if (cycle[start.second][start.first + 1] == next) return std::make_pair(start.second, start.first + 1);
	else if (cycle[start.second - 1][start.first] == next) return std::make_pair(start.second - 1, start.first);
	else if (cycle[start.second][start.first - 1] == next) return std::make_pair(start.second, start.first - 1);
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
	}
	delete tempPath;
	return temp;
}