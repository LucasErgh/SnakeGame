#include <utility> // std::pair

#include "Direction.h" // Direction Enum
#include "SnakeNode.h"

#pragma once
class Snake
{
public:
	Snake(cords start);
	// Constructor creates snake with one node at given cordinate

	cords move();
	// This function moves the snake one unit in current direction
	// returns the location of the snake head

	bool isSnake(cords cords);
	// given any cordinate checks if it collides with snake

	bool headCollision(cords cords);
	// returns true if given cordinates are the heads cordinates returns false otherwise

	void grow(int growthAmount);
	// Increases snake max size so snake grows

	void changeDirection(Direction dir);
	// Changes snake Direction

private:
	SnakeNode* head;
	SnakeNode* tail;
	Direction direction;
	int size;
	int maxSize;
};