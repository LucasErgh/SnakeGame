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
	right,
	none
};

static bool IsOposite(Direction dir1, Direction dir2) {
	switch (dir1)
	{
	case up:
		if (dir2 == down) return true;
		break;
	case down:
		if (dir2 == up) return true;
		break;
	case left:
		if (dir2 == right) return true;
		break;
	case right:
		if (dir2 == left) return true;
		break;
	case none:
		break;
	default:
		break;
	}
	return false;
}

typedef std::pair<int, int> cords;

typedef std::pair<cords, CellType> cell;

typedef std::vector<cell> CellList;

