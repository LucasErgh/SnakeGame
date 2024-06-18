#include <random>

#include "Apple.h"

Apple::Apple() {
	srand(time(0));
}

cords Apple::moveApple(int width, int height) {
	
	cordinates = std::make_pair(((rand() % height) + 1), ((rand() % width) + 1));
	return cordinates;
}

cords Apple::getCords() {
	return cordinates;
}