#pragma once

#include "Snake.h"
#include "Apple.h"
#include "DataTypes.h"

class ControlInterface
{
	ControlInterface(int width = 10, int height = 10);

	virtual bool DoTurn() = 0;

	virtual void ChangeDirection() = 0;

	virtual void endGame() = 0;

	CellList GetCells();

	bool isAlive();

	int Score();

private:


	const int growthRate = 1;
	int width, height;
	Snake snake;
	Apple apple;
	bool gameOver;
};

