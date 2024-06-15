#pragma once

#include "Board.h"

struct StateInfo {
    Board* board;
    int rows, columns;
    int scale;

    StateInfo(int rows, int columns, int scale, Board* board) : rows(rows), columns(columns), scale(scale), board(board) {  }
    CellList GetCells() { return board->updateBoard(); }
};