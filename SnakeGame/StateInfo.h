#pragma once

#include "ControlInterface.h"

struct StateInfo {
	ControlInterface* model;
	int rows, columns;
	int scale;

	StateInfo(int rows, int columns, int scale, ControlInterface* board) : rows(rows), columns(columns), scale(scale), model(board) {  }
	CellList GetCells() { return model->GetCells(); }
};

