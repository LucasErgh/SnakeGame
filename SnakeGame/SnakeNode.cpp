#include "SnakeNode.h"

SnakeNode* SnakeNode::getFront() {
	return front;
}

SnakeNode* SnakeNode::getBack() {
	return back;
}

cords SnakeNode::getCords() {
	return cordinates;
}

void SnakeNode::newFront(SnakeNode* newFront) {
	this->front = newFront;
}

void SnakeNode::newBack(SnakeNode* newBack) {
	this->back = newBack;
}

SnakeNode::SnakeNode(cords cordinates, SnakeNode* front, SnakeNode* back) {
	this->cordinates = cordinates;
	this->front = front;
	this->back = back;
}