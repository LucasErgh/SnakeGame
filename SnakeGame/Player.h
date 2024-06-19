#pragma once

#include "ControlInterface.h"

class Player : public ControlInterface
{
public:
	Player(int width = 10, int height = 10);

	bool ControlInterface::DoTurn();

	void ControlInterface::ChangeDirection(Direction);

	void ControlInterface::endGame();
};

