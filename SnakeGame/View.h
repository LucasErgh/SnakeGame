#include "DataTypes.h"
#include "CellList.h"

#pragma once
class View
{
public:
	virtual void update(int height, int width, CellList board) = 0;
};

