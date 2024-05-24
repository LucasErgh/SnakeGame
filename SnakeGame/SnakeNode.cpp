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
	newFront->back = this;
}

void SnakeNode::newBack(SnakeNode* newBack) {
	this->back = newBack;
	if (newBack != nullptr)
		newBack->front = this;
}

SnakeNode::SnakeNode(cords cordinates, SnakeNode* front, SnakeNode* back) {
	this->cordinates = cordinates;
	this->front = front;
	this->back = back;
}