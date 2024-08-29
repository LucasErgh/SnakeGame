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
	int width, height;
	Snake* snake;

	// This will make a copy of the snake for my pathfinders to use
	void UpdateSnake(Snake* snake);
};

