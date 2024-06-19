#include "Player.h"
#include "ControlInterface.h"

Player::Player(int width, int height){
	snake = Snake(std::make_pair(width / 2, height / 2));
	this->width = width;
	this->height = height;
	this->gameOver = false;

	apple = Apple();
	do {
		apple.moveApple(this->width, this->height);
	} while (snake.isSnake(apple.getCords()));
}

bool Player::DoTurn() {
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

void Player::ChangeDirection(Direction dir) {
	snake.changeDirection(dir);
}

void Player::endGame() {
	snake.deleteNodes();
}