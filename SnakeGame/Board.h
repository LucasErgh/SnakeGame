#pragma once

#include <vector>
#include "Snake.h"
#include "Apple.h"
#include "CellList.h"

class Board
{
public:
	Board(int width = 20, int height = 20);

	bool doTurn();

	bool doTurn(Direction dir);

	CellList updateBoard();

	void endGame();

private:
	int width, height;
	Snake snake;
	Apple apple;
	const int growthRate = 3;
	CellList board;
};
