#include "Player.h"
#include "ControlInterface.h"

Player::Player(int width, int height){
	snake = Snake(std::make_pair(width / 2 + 1, height / 2 + 1), width, height);
	this->width = width;
	this->height = height;
	this->gameOver = false;

	apple = Apple();
	do {
		apple.moveApple(this->width, this->height);
	} while (snake.isSnake(apple.getCords()));
}

bool Player::DoTurn() {
	// Change direction if needed and move snake
	if (queuedDirection1 != none) {
		snake.changeDirection(queuedDirection1);
		queuedDirection1 = queuedDirection2;
		queuedDirection2 = none;
	}
	snake.move();

	// Check for apple collision, grow and make new apple if so
	if (snake.headLocation() == apple.getCords()) {
		snake.grow(growthRate);
		do {
			apple.moveApple(width, height);
		} while (snake.isSnake(apple.getCords()));
	}

	//Check for wall collision and snake collision
	if (snake.died()) {
		gameOver = true;
		return false;
	}
	return true;
}

void Player::ChangeDirection(Direction dir) {
	if (queuedDirection1 == none && !IsOposite(dir, snake.GetDirection())) {
		queuedDirection1 = dir;
		return;
	}
	else if (queuedDirection1 != none && !IsOposite(dir, queuedDirection1)) {
		queuedDirection2 = dir;
	}
}

void Player::endGame() {
	snake.deleteNodes();
}