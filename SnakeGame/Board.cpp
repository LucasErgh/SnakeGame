#include <tuple>
#include <utility>
#include "Board.h"

Board::Board(int width, int height) : width(width), height(height) {
	snake = Snake(std::make_pair(height/2, width/2));

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

	//Check for wall collision and snake collision
	if (snake.snakeCollision())
		return false;
	else if (snake.outOfBounds(width, height))
		return false;
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

void Board::endGame() {
	snake.deleteNodes();
}