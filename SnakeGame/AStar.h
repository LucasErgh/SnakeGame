#pragma once

#include "DataTypes.h"
#include "Snake.h"

class Node {
public:
	Direction dir;
	Node* parent;
	cords pos;
	int G; // Cost from initial state
	int H; // Estimated cost of cheapest path

	Node(cords cords, Node* parent, Direction direction);

	// Sum of G and H
	int GetScore();
};

typedef std::vector<Node*> NodeSet;

class AStar
{
public:
	AStar(Snake* snake);

	void updateSnake(Snake* snake);

	std::vector<Direction>* FindPath(cords goal);

private:
	int size;
	int height, width;
	CordList walls;
	cords start;
	Direction startDir;

	// Cechs list of walls for collisions
	bool collision(cords check, int movement);

	// moves given cell in given direction one unit
	void Shift(Direction dir, cords& cur);

	// Returns node with given cords from set of all nodes
	Node* findNode(NodeSet& nodes, cords cur);

	int distance(cords cords1, cords cords2);

	std::vector<Direction>* getDirections(CordList cordList);
};

