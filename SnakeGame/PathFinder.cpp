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

	 
	// Try to find the A* path
	std::vector<Direction>* path = star.FindPath(goal);

	// Check if its safe
	if (safe(path))
	{
		return path;
	}
	// if not follow cycle
	else {
		Snake* simulatedSnake = SimulateMove(path);
		std::vector<Direction>* cyclePath = RejoinCycle(simulatedSnake);
		
		if (cyclePath && safe(cyclePath)) {
			// Append cyclePath to path
			int size = cyclePath->size();
			for (int i = 0; i < size; ++i) {
				path->insert(path->begin(), cyclePath->back());
				cyclePath->pop_back();
			}
		}

		delete simulatedSnake;

		if (path && safe(path)) {
			delete cyclePath;
			return path;
		}

		if (cyclePath && safe(cyclePath)) {
			delete path;  // Clean up the previous unsafe path
			return cyclePath;  // Return the safe cycle path
		}

		delete cyclePath;

		// Try to rejoin the cycle from the current snake position
		cyclePath = RejoinCycle(snake);
		if (cyclePath && safe(cyclePath)) {
			delete path;  // Clean up the previous unsafe path
			return cyclePath;  // Return the safe path
		}
	}
	delete path;  // Clean up the previous path
	throw (1); //Handle the case where no safe path is found

	
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
	if (!path || path->empty()) return false;

		// Get the snake after the move so we can test for saftey
		Snake temp;
		temp = *SimulateMove(path);

		// Now check if we can travel on the cycle for a number of moves that
		// grows exponentially based on the size of the snake
		int turns = 12;// std::max(12.0, std::pow(2, snake->size / 10));
	
		for (int i = 12; i >= 0; --i) {
			if (temp.died() || IsOposite(temp.direction, cycle.nextDir(temp.headLocation()))) 
				return false;
			temp.changeDirection(cycle.nextDir(temp.headLocation()));
			temp.move();
		}
		
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

std::vector<Direction>* PathFinder::RejoinCycle(Snake* snake) {
	std::vector<cords> nodes;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			nodes.push_back(std::make_pair(j, i));
		}
	}

	bool safepath = false;
	int minDistance = height * width, tempd;

	std::vector<Direction>* path = NULL;

	while (!nodes.empty()) {
		
		cords closestNode;
		int closestNodeIndex = -1;

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
		star.updateSnake(snake);
		path = star.FindPath(closestNode);
		if (path && safe(path)) {
			/*for (int i = 0; i < snake->size; ++i) {
				path->insert(path->begin(), cycle.nextDir(closestNode));
				cycle.Shift(path->front(), closestNode.second, closestNode.first);
			}*/
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
		if (temp->headLocation() == goal) {
			temp->grow(1);
		}
	}
	delete tempPath;
	return temp;
}