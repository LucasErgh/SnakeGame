#pragma once

#include "Snake.h"
#include "DataTypes.h"

class ControlInterface
{
public:
	//ControlInterface(int width = 10, int height = 10);

	virtual bool DoTurn() = 0;

	virtual void ChangeDirection(Direction) = 0;

	virtual void endGame() = 0;

	CellList GetCells();

	bool isAlive();

	int Score();

protected:
	const int growthRate = 1;
	int width, height;
	Snake snake;
	bool gameOver;
};

