#pragma once

#include "DataTypes.h"
#include "Snake.h"

class HamCycle {
public:
	HamCycle(int height, int width);

	// This generates a hamilton cycle with maximum turns of given size
	// With the requirnment that width and height are divisible by four
	// and width and height are the same value
	int** GenerateCycle();
	
	// This will get the path
	std::vector<Direction>* FindPath(Snake* snake, cords goal);

	// This is just to delete the cycle array when the object is deleted
	~HamCycle();

	// Overloading index operator so we can access 
	// the array directly after it has been generated
	int* operator[] (int row);
	const int* operator[] (int colm) const;

	// returns the next direction for a given location
	Direction nextDir(cords cords);

	// returns the direction you have to go for the previous node
	Direction prevDir(cords cords);

	// This shifts cells in the given direction
	void Shift(Direction dir, int& y, int& x);

private:
	int** cycle;
	cords goal;
	int height, width;
	int max;

	// Given the grid "grid" has dimentions (height, width) this will find the 
	// preceeding and next cells and store them in the variables dir1 for the 
	// previous direction, and dir2 for the next direciton. If dir2 or dir2 are
	// the return will represent if they are accurate
	// This also assumes that the node is not empty (grid[y][x] != -1)
	bool GetDirection(int** grid, int y, int x, Direction* dir1, Direction* dir2, int height, int width, int max, int min = 0);

};