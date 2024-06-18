#include <tuple>
#include <utility>
#include "Board.h"

int Board::Score() {
	return snake.getSize();
}

Board::Board(int width, int height) : width(width), height(height) {
	snake = Snake(std::make_pair(height/2, width/2));
	gameOver = false;

	// Make Apple
	apple = Apple();
	do {
		apple.moveApple(width, height);
	} while (snake.isSnake(apple.getCords()));
}

bool Board::isAlive() {
	if (gameOver == true) return false;
	else				  return true;
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
	if (snake.snakeCollision()) {
		gameOver = true;
		return false;
	}
	else if (snake.outOfBounds(width, height)) {
		gameOver = true;
		return false;
	}
	return true;
}

bool Board::doTurn(Direction dir) {
	snake.changeDirection(dir);
	return doTurn();
}

void Board::ChangeDirection(Direction dir) {
	snake.changeDirection(dir);
}

CellList Board::updateBoard() {
	board.Load(apple.getCords(), snake.headLocation(), snake.bodyLocation());
	return board;
}

void Board::endGame() {
	snake.deleteNodes();
}