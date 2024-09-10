#include "Player.h"
#include "ControlInterface.h"

Player::Player(int width, int height){
	snake = Snake(std::make_pair(width / 2 + 1, height / 2 + 1), width, height);
	this->width = width;
	this->height = height;
	this->gameOver = false;
}

bool Player::DoTurn() {
	// Change direction if needed and move snake
	if (queuedDirection1 != none) {
		snake.changeDirection(queuedDirection1);
		queuedDirection1 = queuedDirection2;
		queuedDirection2 = none;
	}
	snake.move();

	// Check if snake Died
	if (!snake.isAlive()) {
		gameOver = true;
		return false;
	}
	return true;
}

void Player::ChangeDirection(Direction dir) {
	if (queuedDirection1 == none && !IsOposite(dir, snake.getDirection())) {
		queuedDirection1 = dir;
		return;
	}
	else if (queuedDirection1 != none && !IsOposite(dir, queuedDirection1)) {
		queuedDirection2 = dir;
	}
}

void Player::endGame() {
	// I don't think there is anything to be done
}