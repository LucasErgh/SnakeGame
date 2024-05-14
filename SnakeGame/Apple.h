#include "DataTypes.h"
#include "Snake.h"

#pragma once
class Apple {
public:
	Apple();

	cords getCords();

	cords moveApple(int width, int height);

private:
	cords cordinates;
};

