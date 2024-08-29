#include "PathFinder.h"

PathFinder::PathFinder(int width, int height) : snake(NULL), width(width), height(height), max(width* height - 1) {
	cycle = GenerateCycle(width, height);
}

std::vector<Direction>* PathFinder::FindPath(Snake* snake, cords goal) {

}

void PathFinder::UpdateSnake(Snake* snake) {
	if (this->snake) {
		this->snake->deleteNodes();
		delete this->snake;
	}

	// Uses overloaded copy constructor to make a copy we can modify
	this->snake = new Snake(*snake);

	// Creates AStar pathfinding object if it doesn't already exist
	// and if it does exist it updates it with the new snake
	if (!aStar) {
		aStar = new AStar(this->snake);
	}
	else {
		aStar->updateSnake(this->snake);
	}
}