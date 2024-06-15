#include "Snake.h"
#include "DataTypes.h"
#include "SnakeNode.h"

Snake::Snake() {
	head = new SnakeNode(std::make_pair(0, 0), nullptr, nullptr);
	tail = head;
	lastDirection = up;
	queuedDirection1 = none;
	queuedDirection2 = none;
	size = 1;
	maxSize = size;
}

Snake::Snake(cords start) {
	head = new SnakeNode(start, nullptr, nullptr);
	tail = head;
	lastDirection = up;
	queuedDirection1 = none;
	queuedDirection2 = none;
	size = 1;
	maxSize = size;
}

cords Snake::move() {
	// Create new had location based on current head location
	cords newLocation(head->getCords());
	Direction direction;

	if (queuedDirection1 == none) 
		direction = lastDirection;
	else {
		direction = queuedDirection1;
		lastDirection = queuedDirection1;
		queuedDirection1 = queuedDirection2;
		queuedDirection2 = none;
	}

	// Move head location by one in current direction
	switch (direction)
	{
	case up:
		--newLocation.second;
		break;
	case down:
		++newLocation.second;
		break;
	case left:
		--newLocation.first;
		break;
	case right:
		++newLocation.first;
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
		if (oldTail->getFront() == nullptr)
			tail = head;
		else {
			tail = oldTail->getFront();
			tail->newBack(nullptr);
		}
		delete oldTail;
	}
	return newHead->getCords();
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
	Direction* last, *next;
	if (queuedDirection1 == none) {
		last = &lastDirection;
		next = &queuedDirection1;
	}
	else {
		last = &queuedDirection1;
		next = &queuedDirection2;
	}
	*next = dir;
	switch (*last)
	{
	case up:
		if (*next == down) *next = none;
		break;
	case down:
		if (*next == up) *next = none;
		break;
	case left:
		if (*next == right) *next = none;
		break;
	case right:
		if (*next == left) *next = none;
		break;
	}
}

cords Snake::headLocation() {
	return head->getCords();
}

std::vector<cords> Snake::bodyLocation() {
	SnakeNode* cur = head;
	std::vector<cords> body;
	for (int i = 1; i < size; ++i) {
		cur = cur->getBack();
		body.push_back(cur->getCords());
	}
	return body;
}

int Snake::getSize() {
	return size;
}

void Snake::deleteNodes() {
	SnakeNode* cur = tail;
	while (cur->getFront() != nullptr) {
		cur = cur->getFront();
		delete cur->getBack();
	}
	delete cur;
}

bool Snake::snakeCollision() {
	std::vector<cords> body = bodyLocation();
	for (auto cur : body) {
		if (head->getCords() == cur)
			return true;
	}
	return false;
}

bool Snake::outOfBounds(int width, int height) {
	cords headCords = headLocation();
	if (headCords.first <= 0 || headCords.second <= 0 || headCords.first > width || headCords.second > height)
		return true;
	return false;
}