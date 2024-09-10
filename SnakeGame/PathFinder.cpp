#include "PathFinder.h"
#include <cmath>

void PathFinder::Delete() {
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
	star.updateSnake(this->snake);
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
			if (!temp->isAlive() || IsOposite(temp->getDirection(), cycle.nextDir(temp->headLocation())))
				return false;
			temp->changeDirection(cycle.nextDir(temp->headLocation()));
			temp->move();
			if (!temp->isAlive() || IsOposite(temp->getDirection(), cycle.nextDir(temp->headLocation())))
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
		star.updateSnake(snake);
		path = star.FindPath(closestNode);
		if (path && safe(path)) {
			/*for (int i = 0; i < snake->size; ++i) {
				path->insert(path->begin(), cycle.nextDir(closestNode));
				cycle.Shift(closestNode, closestNode.second, closestNode.first);
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
		path->insert(path->begin(), cycle.nextDir(cur));
		cycle.Shift((*path)[0], cur.second, cur.first);
	}
	return path;
}