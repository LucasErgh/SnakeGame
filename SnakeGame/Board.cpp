#include <tuple>
#include <utility>
#include "Board.h"

Board::Board(int width = 20, int height = 20) : width(width), height(height) {
	snake = Snake(std::make_pair(width/2, height/2));

	// Make Apple
	apple = Apple();
	do {
		apple.moveApple(width, height);
	} while (snake.isSnake(apple.getCords()));
}

bool Board::doTurn() {
	snake.move();
	
	// Check for apple collision, grow and make new apple if so
	if (snake.headCollision(apple.getCords())) {
		snake.grow(growthRate);
		do {
			apple.moveApple(width, height);
		} while (snake.isSnake(apple.getCords()));
	}

	//Check for wall collision
	cords head = snake.headLocation();
	if (head.first < 0 || head.first >= width ||
		head.second < 0 || head.second >= height) {
		return false;
	}
	return true;
}

bool Board::doTurn(Direction dir) {
	snake.changeDirection(dir);
	return doTurn();
}

CellList Board::updateBoard() {
	board.Load(apple.getCords(), snake.headLocation(), snake.bodyLocation());
	return board;
}