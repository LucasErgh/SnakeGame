#pragma once

#include <vector>

#include "ControlInterface.h"

class ComputerPlayer : public ControlInterface
{
public:
	ComputerPlayer(int width = 10, int height = 10);

	bool ControlInterface::DoTurn();

	void ControlInterface::ChangeDirection(Direction);

	void ControlInterface::endGame();

	std::vector<Direction>* directions;
};

