#include <utility>
#include <vector>
#include <tuple>

#pragma once
enum CellType {
	appleCell,
	headCell,
	bodyCell,
};

enum Direction {
	up,
	down,
	left,
	right
};

typedef std::pair<int, int> cords;

typedef std::pair<cords, CellType> cell;

