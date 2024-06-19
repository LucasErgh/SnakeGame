#include <utility>
#include <vector>
#include <tuple>

#include "ControlInterface.h"

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

typedef std::pair<int, int> cords;

typedef std::pair<cords, CellType> cell;

typedef std::vector<cell> CellList;

struct StateInfo {
	ControlInterface* model;
	int rows, columns;
	int scale;

	StateInfo(int rows, int columns, int scale, ControlInterface* board) : rows(rows), columns(columns), scale(scale), model(board) {  }
	CellList GetCells() { return model->GetCells(); }
};