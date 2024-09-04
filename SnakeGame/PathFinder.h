#pragma once

#include "HamCycle.h"
#include "AStar.h"
#include "Snake.h"
#include "DataTypes.h"

typedef std::vector<cords> CordList;

class PathFinder
{
public:
	// Dimentions are needed to create the hamilton cycle
	PathFinder(int width, int height);

	// Returns set of directions for snake to follow
	std::vector<Direction>* FindPath(Snake* snake, cords goal);

	void Delete();

private:
	HamCycle cycle;
	AStar star;
	int width, height, max;
	Snake* snake;
	cords goal;

	// This will make a copy of the snake for my pathfinders to use
	void UpdateSnake(Snake* snake);

	// This will simulate the snakes moves on the given set of directions
	// and then check to see if the snake can continue on the hamilton cycle
	bool safe(std::vector<Direction>* path);

	// This function gives the cordinates of the cell that is next in the cycle
	cords nextCycleNode(cords);

	// This will keep attempting to rejoing the cycle at the nearest node and if
	// it fails it will try the next nearest and so on untill it succeeds
	std::vector<Direction>* RejoinCycle(Snake* snake, int radius);

	// This will make a copy of the active snake and make that snake 
	// move through the given path for the purposes of testing if its safe
	Snake* SimulateMove(std::vector<Direction>* path);

	int distance(cords a, cords b);
};

