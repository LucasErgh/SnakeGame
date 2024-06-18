#pragma once

#include <vector>
#include "Snake.h"
#include "Apple.h"

class Board
{
public:
	Board(int width = 20, int height = 20);

	bool doTurn();

	bool doTurn(Direction dir);

	void ChangeDirection(Direction dir);

	CellList updateBoard();

	void endGame();

	bool isAlive();

	int Score();

private:
	int width, height;
	Snake snake;
	Apple apple;
	const int growthRate = 1;
	CellList board;
	bool gameOver;
};
