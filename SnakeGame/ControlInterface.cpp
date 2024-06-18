#include "ControlInterface.h"

ControlInterface::ControlInterface(int width, int height) : width(width), height(height){
	snake = Snake(std::make_pair(width / 2, height / 2));

	apple = Apple();
	do {
		apple.moveApple(this->width, this->height);
	} while (snake.isSnake(apple.getCords()));
}

CellList ControlInterface::GetCells() {
	CellList list;
	list.clear();

	list.push_back(std::make_pair(apple.getCords(), appleCell));
	list.push_back(std::make_pair(snake.headLocation(), headCell));
	for (auto cur : snake.bodyLocation()) {
		list.push_back(std::make_pair(cur, bodyCell));
	}
}
bool ControlInterface::isAlive() {
	return !gameOver;
}

int ControlInterface::Score() {
	return snake.getSize();
}