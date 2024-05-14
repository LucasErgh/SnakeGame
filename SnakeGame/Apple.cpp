#include <random>

#include "Apple.h"

Apple::Apple(int width, int height, Snake snake) {
	srand(time(0));
	do { // creates ordered pairs untill they don't collide with the snake
		cordinates = std::make_pair((rand() % (width + 1)), (rand() % (height + 1)));
	} while (snake.isSnake(cordinates));
}