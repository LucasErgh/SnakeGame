#include "PathFinder.h"

void PathFinder::Delete() {
	snake->deleteNodes();
	delete snake;
}

PathFinder::PathFinder(int width, int height) : cycle(HamCycle(width, height)), star(AStar(snake)), snake(NULL), width(width), height(height) {}

std::vector<Direction>* PathFinder::FindPath(Snake* newSnake, cords goal) {
	UpdateSnake(newSnake);
	star.updateSnake(snake);
	std::vector<Direction>* path = star.FindPath(goal);
	// std::vector<Direction>* path = cycle.FindPath(snake, goal);

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