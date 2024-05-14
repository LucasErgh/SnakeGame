#pragma once

#include "DataTypes.h"
#include <vector>

class CellList
{
public:
	void Load(cords apple, cords head, std::vector<cords> body);

	void Order();

	std::vector<cell> getList();

private:
	std::vector<cell> list;
};

