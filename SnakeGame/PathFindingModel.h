#pragma once

#include "Snake.h"
#include "DataTypes.h"

class PathFindingModel
{
	virtual std::vector<Direction>* FindPath() = 0;
};

