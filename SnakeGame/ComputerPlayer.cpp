#include "ComputerPlayer.h"


ComputerPlayer::ComputerPlayer(int width, int height) : directions(NULL){
	snake = Snake(std::make_pair(width / 2 + 1, height / 2 + 1), width, height);
	this->width = width;
	this->height = height;
	this->gameOver = false;

	path = new PathFinder(width, height);
}

bool ComputerPlayer::DoTurn() {
	if (directions != NULL && (directions->empty())) {
		delete directions;
		directions = NULL;
	}
	if (directions == NULL) {
		directions = path->FindPath(&snake, snake.apple.getCords());
	}
	if (directions != NULL && directions->size() != 0) {
		Direction direction = directions->back();
		directions->pop_back();
		snake.changeDirection(direction);
	}

	snake.move();

	if (!snake.alive) {
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