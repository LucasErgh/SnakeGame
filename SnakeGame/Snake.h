#pragma once

#include <utility> // std::pair
#include "DataTypes.h"


class Apple {
public:
	Apple();

	cords getCords();

	cords moveApple(int width, int height);

private:
	cords cordinates;
};

class SnakeNode {
public:
	SnakeNode(cords cordinates, SnakeNode* front, SnakeNode* back);
	SnakeNode* front;
	SnakeNode* back;
	cords cordinates;
};

class Snake
{
public:
	Snake();
	// If no start cords are given starts at (0, 0)

	Snake(cords start, int width, int height);
	// Constructor creates snake with one node at given cordinate

	Snake(const Snake& snake);
	// Copy Constructor

	~Snake();
	// Destructor

	Snake& operator=(const Snake& snake);
	// Assignment operator for deep coppies

	cords move();
	// This function moves the snake one unit in current direction
	// returns the location of the snake head

	bool isSnake(cords cords);
	// given any cordinate checks if it collides with snake

	void grow(int growthAmount);
	// Increases snake max size so snake grows

	void changeDirection(Direction dir);
	// Changes snake Direction

	cords headLocation();
	// Returns current head location

	std::vector<cords> bodyLocation();
	// Returns list of body cords

	bool isAlive();
	// returns whether or not the snake is alive

	cords appleCords();
	// returns apple's cords

	Direction getDirection();
	// returns snakes current direction

	int getSize();
	// returns snakes size

	int width;
	int height;

private:
	void deleteNodes();
	// Deletes all nodes
	
	SnakeNode *head;
	SnakeNode *tail;

	Direction direction;
	Apple apple;

	int maxSize;
	int size;
	bool alive;
};
