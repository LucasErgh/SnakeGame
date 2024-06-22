#include "ComputerPlayer.h"


ComputerPlayer::ComputerPlayer(int width, int height) : directions(NULL){
	snake = Snake(std::make_pair(width / 2, height / 2), width, height);
	this->width = width;
	this->height = height;
	this->gameOver = false;

	apple = Apple();
	do {
		apple.moveApple(this->width, this->height);
	} while (snake.isSnake(apple.getCords()));
}

bool ComputerPlayer::DoTurn() {
	if (directions != NULL && (directions->empty() || directions->size() == 1)) {
		delete directions;
		directions = NULL;
	}
	if (directions == NULL) {
		PathFindingModel* model = new PathFinderV2(&snake, apple.getCords());
		directions = model->FindPath();
	}
	if (directions != NULL) {
		Direction direction = directions->back();
		directions->pop_back();
		snake.changeDirection(direction);
	}

	snake.move();

	if (snake.headLocation() == apple.getCords()) {
		snake.grow(growthRate);
		do {
			apple.moveApple(width, height);
		} while (snake.isSnake(apple.getCords()));
	}
	if (snake.outOfBounds() || snake.snakeCollision()) {
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