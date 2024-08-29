#include "ComputerPlayer.h"


ComputerPlayer::ComputerPlayer(int width, int height) : directions(NULL){
	snake = Snake(std::make_pair(width / 2 + 1, height / 2 + 1), width, height);
	this->width = width;
	this->height = height;
	this->gameOver = false;

	apple = Apple();
	do {
		apple.moveApple(this->width, this->height);
	} while (snake.isSnake(apple.getCords()));

	path = new PathFinderV4(&snake);
}

bool ComputerPlayer::DoTurn() {
	if (directions != NULL && (directions->empty())) {
		delete directions;
		directions = NULL;
	}
	if (directions == NULL) {
		//path = new PathFinderV3(&snake);
		directions = path->FindPath(apple.getCords());
	}
	if (directions != NULL && directions->size() != 0) {
		Direction direction = directions->back();
		directions->pop_back();
		snake.changeDirection(direction);
	}

	snake.move();

	if (snake.headLocation() == apple.getCords()) {
		snake.grow(growthRate);
		if (snake.size == height * width) {
			path->Delete();
			delete path;
			gameOver = true;
			return false;
		}
		do {
			apple.moveApple(width, height);
		} while (snake.isSnake(apple.getCords()));
	}
	if (snake.died()) {
		path->Delete();
		gameOver = true;
		return false;
	}
	else return true;	
}

void ComputerPlayer::ChangeDirection(Direction) {
	// This does nothing because the computer player controls itself
}

void ComputerPlayer::endGame() {
		snake.deleteNodes();
}