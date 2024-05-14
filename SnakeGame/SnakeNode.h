#include "Direction.h"

#pragma once
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

