#include "Snake.h"
#include "DataTypes.h"

#include <time.h>

Snake::Snake() {
	alive = true;
	height = 10;
	width = 10;
	head = new SnakeNode(std::make_pair(1, 1), nullptr, nullptr);
	tail = head;
	direction = none;
	size = 1;
	maxSize = size;
	do {
		apple.moveApple(width, height);
	} while (isSnake(apple.getCords()));
}

Snake::Snake(cords start, int width, int height) : width(width), height(height), alive(true) {
	head = new SnakeNode(start, nullptr, nullptr);
	tail = head;
	direction = none;
	size = 1;
	maxSize = size;
	apple.moveApple(width, height);
}

Snake::Snake(const Snake& snake) {
	height = snake.height;
	width = snake.width;
	maxSize = snake.maxSize;
	size = snake.size;
	direction = snake.direction;
	alive = snake.alive;
	apple = snake.apple;

	SnakeNode* cur = snake.head;
	head = new SnakeNode(cur->cordinates, NULL, NULL);
	SnakeNode* newCur = head;
	while (cur->back != NULL) {
		cur = cur->back;
		newCur->back = new SnakeNode(cur->cordinates, newCur, NULL);
		newCur->back->front = newCur;
		newCur = newCur->back;
	}
	tail = newCur;
}

Snake::~Snake() {
	deleteNodes();
}

Snake& Snake::operator=(const Snake& snake) {
	height = snake.height;
	width = snake.width;
	maxSize = snake.maxSize;
	size = snake.size;
	direction = snake.direction;
	alive = snake.alive;
	apple = snake.apple;

	SnakeNode* cur = snake.head;
	head = new SnakeNode(cur->cordinates, NULL, NULL);
	SnakeNode* newCur = head;
	while (cur->back != NULL) {
		cur = cur->back;
		newCur->back = new SnakeNode(cur->cordinates, newCur, NULL);
		newCur->back->front = newCur;
		newCur = newCur->back;
	}
	tail = newCur;

	return *this;
}

cords Snake::appleCords() {
	return apple.getCords();
}

cords Snake::move() {
	if (direction == none) return head->cordinates;

	// Create new had location based on current head location
	cords newLocation(head->cordinates);

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
	head->front = newHead;
	head = newHead;

	// Grow snake and move tail
	if (maxSize > size) { // if snake needs to grow just increase size don't delete tail
		++size;
	}
	else { // if the snake doesn't need to grow make the tail the node infront of tail and delete the old tail
		SnakeNode* oldTail = tail;
		if (oldTail->front == nullptr)
			tail = head;
		else {
			tail = oldTail->front;
			tail->back = NULL;
		}
		delete oldTail;
	}

	// Check if we ate the apple
	if (newHead->cordinates == apple.getCords()) {
		++maxSize;
		if (size == height * width) {
			alive = false;
			return newHead->cordinates;
		}
		do {
			apple.moveApple(width, height);
		} while (isSnake(apple.getCords()));
	}

	// Check for collitsion
	if (head->cordinates.first <= 0 || head->cordinates.second <= 0 || head->cordinates.first > width || head->cordinates.second > height) 
		alive = false;
	for (auto& cur : bodyLocation()) {
		if (head->cordinates == cur) alive = false;
	}

	return newHead->cordinates;
}

bool Snake::isSnake(cords cords) {
	SnakeNode* cur = head;
	if (cur->cordinates == cords) {
		return true;
	}
	while (true) {
		if (cur->back == nullptr) { // no collisions
			return false;
		}
		if (cur->back->cordinates == cords) { // found a collision
			return true;
		}
		cur = cur->back;
	}
}

void Snake::grow(int growthAmount) {
	maxSize += growthAmount;
}

void Snake::changeDirection(Direction dir) {
	if (!IsOposite(dir, direction)) {
		direction = dir;
	}
}

cords Snake::headLocation() {
	return head->cordinates;
}

std::vector<cords> Snake::bodyLocation() {
	SnakeNode* cur = head;
	std::vector<cords> body;
	for (int i = 1; i < size; ++i) {
		cur = cur->back;
		body.push_back(cur->cordinates);
	}
	return body;
}

void Snake::deleteNodes() {
	SnakeNode* cur = tail;
	while (cur->front != nullptr) {
		cur = cur->front;
		delete cur->back;
	}
	delete cur;
}

Direction Snake::getDirection() { return this->direction; }

bool Snake::isAlive() { return alive; }

int Snake::getSize() { return size; }

SnakeNode::SnakeNode(cords cordinates, SnakeNode* front, SnakeNode* back) {
	this->cordinates = cordinates;
	this->front = front;
	this->back = back;
}

Apple::Apple() {
	srand(time(0));
}

cords Apple::moveApple(int width, int height) {

	cordinates = std::make_pair(((rand() % width) + 1), ((rand() % height) + 1));
	return cordinates;
}

cords Apple::getCords() {
	return cordinates;
}