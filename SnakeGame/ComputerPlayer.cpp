#include "ComputerPlayer.h"

ComputerPlayer::ComputerPlayer(int width, int height) {
	snake = Snake(std::make_pair(width / 2, height / 2));
	this->width = width;
	this->height = height;
	this->gameOver = false;

	apple = Apple();
	do {
		apple.moveApple(this->width, this->height);
	} while (snake.isSnake(apple.getCords()));
}

bool ComputerPlayer::DoTurn() {
	return true;
}

void ComputerPlayer::ChangeDirection(Direction) {
	// This does nothing because the computer player controls itself
}

void ComputerPlayer::endGame() {
	snake.deleteNodes();
}