#pragma once

#include "ControlInterface.h"

struct StateInfo {
	ControlInterface* model1;
	ControlInterface* model2;
	int rows, columns;
	int scale;
	int gamemode;
	bool ingame;

	StateInfo(int columns, int rows, int scale, ControlInterface* board, ControlInterface* board2 = NULL) : 
		rows(rows), columns(columns), scale(scale), model1(board), model2(board2) {  }

	std::pair<CellList, CellList> GetCells() 
	{ 
		if (model2) {
			return std::make_pair<CellList, CellList>(model1->GetCells(), model2->GetCells());
		}
		return std::make_pair<CellList, CellList>(model1->GetCells(), model1->GetCells());
	}
};

