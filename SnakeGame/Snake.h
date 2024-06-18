#include <utility> // std::pair

#include "DataTypes.h"

#pragma once
class Snake
{
public:
	Snake();
	// If no start cords are given starts at (0, 0)

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

	cords headLocation();
	// Returns current head location

	std::vector<cords> bodyLocation();
	// Returns list of body cords

	int getSize();
	// Returns current size of snake

	void deleteNodes();
	// Deletes all nodes

	bool snakeCollision();

	bool outOfBounds(int width, int height);

private:
	SnakeNode* head;
	SnakeNode* tail;
	Direction lastDirection;
	Direction queuedDirection1;
	Direction queuedDirection2;
	int size;
	int maxSize;
};

class SnakeNode {
public:
	SnakeNode(cords cordinates, SnakeNode* front, SnakeNode* back);
	SnakeNode* getFront();
	SnakeNode* getBack();
	void newFront(SnakeNode* newFront);
	void newBack(SnakeNode* newBack);
	cords getCords();

private:
	SnakeNode* front;
	SnakeNode* back;
	cords cordinates;
};

class Apple {
public:
	Apple();

	cords getCords();

	cords moveApple(int width, int height);

private:
	cords cordinates;
};