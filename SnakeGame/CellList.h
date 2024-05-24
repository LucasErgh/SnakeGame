#pragma once

#include "DataTypes.h"
#include <vector>

class CellList
{
public:
	void Load(cords apple, cords head, std::vector<cords> body);

	void Order();

	std::vector<cell> getList();

	int x(int index);

	int y(int index);

	CellType type(int index);

	int size();

private:
	std::vector<cell> list;
};

