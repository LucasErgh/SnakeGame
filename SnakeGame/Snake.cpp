#include "Snake.h"
#include "Direction.h"
#include "SnakeNode.h"

Snake::Snake(cords start) {
	head = new SnakeNode(start, nullptr, nullptr);
	tail = head;
	direction = up;
	size = 1;
	maxSize = size;
}

cords Snake::move() {
	// Create new had location based on current head location
	cords newLocation(head->getCords());

	// Move head location by one in current direction
	switch (direction)
	{
	case up:
		++newLocation.second;
		break;
	case down:
		--newLocation.second;
		break;
	case left:
		--newLocation.first;
		break;
	case right:
		++newLocation.second;
		break;
	default:
		break;
	}

	// Create new Head with new cords
	SnakeNode* newHead = new SnakeNode(newLocation, nullptr, head);
	head->newFront(newHead);
	head = newHead;

	// Grow snake and move tail
	if (maxSize > size) { // if snake needs to grow just increase size don't delete tail
		++size;
	}
	else { // if the snake doesn't need to grow make the tail the node infront of tail and delete the old tail
		SnakeNode* oldTail = tail;
		tail = oldTail->getFront();
		delete oldTail;
	}
}

bool Snake::isSnake(cords cords) {
	SnakeNode* cur = head;
	while (true) {
		if (cur->getCords() == cords) { // found a collision
			return true;
		}
		if (cur->getBack() == nullptr) { // no collisions
			return false;
		}
		cur = cur->getBack();
	}
}

bool Snake::headCollision(cords cords) {
	if (head->getCords() == cords) {
		return true;
	}
	else {
		return false;
	}
}

void Snake::grow(int growthAmount) {
	maxSize += growthAmount;
}

void Snake::changeDirection(Direction dir) {
	direction = dir;
}

