#pragma once

#include "Snake.h"
#include "DataTypes.h"

class PathFindingModel
{
public:
	virtual std::vector<Direction>* FindPath() = 0;
};

