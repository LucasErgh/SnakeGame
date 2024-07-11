#pragma once
#include "PathFindingModel.h"

/*
This method will use hamiltonian cycles. The biggest problem 
with hamiltonian is that they can only be constructed if one
of the side lengths of the grid is even. So the user won't be 
able to choose any size game board with this method. 
*/

class PathFinderV4 : public PathFindingModel
{
public:
	int width, height;
	int** cycle; // dynamic array to store cycle (each cell will represent the order in the cycle of that cell)

	PathFinderV4(Snake* snake) : width(snake->width), height(snake->height) {
		cycle = GenerateCycle(width, height);
	}

	std::vector<Direction>* PathFindingModel::FindPath() {
		return NULL;
	}

	void PathFindingModel::Delete() {

	}


private:
	int** GenerateCycle(int width, int height) {
		// At the moment this only works for square grids with side lengths divisible by four
		if (width != height || width % 2 != 0) {
			throw(1);
		}
		
		// The method I'm using for creating the cycles was taken from 
		// a paper called "Turns in Hamilton cycles of rectangular grids" 
		// written by Ethan Y. Tan and Guowen Zhang

		// We start by making a cycle with minimum number of turns for a grid of half its length and width
		int minLength = width / 2;
		int** minimum = new int*[minLength];
		for (int i = 0; i < minLength; ++i) {
			minimum[i] = new int[minLength];
		}

		// Now we create the "pronged" cycle with half side lengths
		int cur = 0;
		minimum[0][0] = cur++;
		for (int i = 0; i < minLength; i += 2) {
			for (int j = 1; j < minLength; ++j) {
				minimum[i][j] = cur++;
			}
			for (int k = minLength - 1; k > 0; --k) {
				minimum[i + 1][k] = cur++;
			}
		}
		for (int i = minLength - 1; i > 0; --i) {
			minimum[i][0] = cur++;
		}

		// Now we must dialate the cycle from (0, 0) by a factor of 2
		// with two added rules. For each turn reduce its x-cordinate
		// by 1 if its vertical segment is oriented up, and reduce its
		// y-cordinate by 1 if its horizontal segment is oriented right.
		// We must also rotate it clockwise before we shift segments.

		// allocate memory for the dilated cycle
		int** minDilated;
		int length = width;
		minDilated = new int* [length];
		for (int i = 0; i < length; ++i) {
			minDilated[i] = new int[length];
			for (int j = 0; j < length; ++j) {
				minDilated[i][j] = -1; // initialize to -1 for debugging
			}
		}

		// Variables to keep track of current position and direction
		Direction enter, exit = none;
		int Ox1 = 0, Oy1 = 0, Ox2 = 0, Oy2 = 0;
		int Nx1, Ny1, Nx2, Ny2;
		int number = 0;
		bool running = true;
		int val = 0;
		int startx, starty;

		// Find starting point
		if (minimum[0][0] != 0) {
			for (int j = 0; j < minLength; ++j) {
				for (int k = 0; k < minLength; ++k) {
					if (minimum[j][k] == val) {
						Oy1 = j;
						Ox1 = k;
						break;
					}
				}
			}
		}

		// Find starting point
		if (Oy1 != minLength - 1 && minimum[Oy1 + 1][Ox1] == minLength * minLength - 1) enter = up;
		else if (Ox1 != minLength - 1 && minimum[Oy1][Ox1 + 1] == minLength * minLength - 1) enter = left;
		else if (Oy1 != 0 && minimum[Oy1 - 1][Ox1] == minLength * minLength - 1) enter = down;
		else if (Ox1 != 0 && minimum[Oy1][Ox1 - 1] == minLength * minLength - 1) enter = left;
		else throw(1);

		Ox2 = Ox1;
		Oy2 = Oy1;

		while (running) {
			// Loop to find the next turn
			do {
				int next = (val >= minLength * minLength - 1) ? val - (minLength * minLength - 1)  : val + 1;

				if (!GetDirection(minimum, Oy2, Ox2, enter, exit, minLength, minLength, minLength * minLength - 1)) {
					throw(1); // If enterance isn't correct throw
				}
				/* Old Version Without Helper
				if (Oy2 < minLength - 1 && minimum[Oy2 + 1][Ox2] == next) {
					exit = down;
					Oy2 += 1;
				}
				else if (Ox2 < minLength - 1 && minimum[Oy2][Ox2 + 1] == next) {
					exit = right;
					Ox2 += 1;
				}
				else if (Oy2 > 0 && minimum[Oy2 - 1][Ox2] == next) {
					exit = up;
					Oy2 -= 1;
				}
				else if (Ox2 > 0 && minimum[Oy2][Ox2 - 1] == next) {
					exit = left;
					Ox2 -= 1;
				}
				else {
					throw(1);
				}
				*/

				++val;
				if (exit == none) {
					throw(1);
				}
			} while (enter == exit);

			Nx2 = Ox2;
			Ny2 = Oy2;

			// undo last move
			switch (exit) {
				case up: ++Ny2; break;
				case down: --Ny2; break;
				case left: ++Nx2; break;
				case right: --Nx2; break;
				default: throw(1); break;
			}

			// scale old points to new points
			Nx2 = Nx2 * 2 + 1;
			Ny2 = Ny2 * 2 + 1;

			// shift points if they traveled right or down
			if (enter == right || exit == right) --Ny2;
			if (exit == down || enter == down) --Nx2;
			
			if (number == 0) {
				Ny1 = Ny2;
				Nx1 = Nx2;
				startx = Nx1;
				starty = Ny1;
				// this will set the both corners to the same value skiping the next loop
				minDilated[Ny1][Nx1] = number++;
			}

			// fill in points between corners
			while ((Nx1 != Nx2 || Ny1 != Ny2)) {

				switch (enter) {
					case up: --Ny1; break;
					case down: ++Ny1; break;
					case left: --Nx1; break;
					case right: ++Nx1; break;
					default: throw(1);  break;
				}			
				if (Nx1 == startx && Ny1 == starty)
					break;
				minDilated[Ny1][Nx1] = number++;
			} 

			// Reset variables
			Ox1 = Ox2;
			Oy1 = Oy2;
			Ny1 = Ny2;
			Nx1 = Nx2;
			enter = exit;
			exit = none;

			// Check if we have completed the cycle
			if (number > 2 && Ny2 == starty && Nx2 == startx) {
				break;
			}
		}

		// Cleam memory
		for (int i = 0; i < minLength; ++i) {
			delete[] minimum[i];
		}
		delete[] minimum;

		/* 
		Now that we have the scaled minimum turns of half the side length grid
		we now start making the maximum turns version by creating a grid of closed
		loops. Then we find the reduced form, which has maximum number of turns.
		*/

		// Initialize both new grids we will need
		int** SquareCycles = new int*[height];
		int** ReducedGrid = new int* [height];

		for (int i = 0; i < height; ++i) {
			SquareCycles[i] = new int[width];
			ReducedGrid[i] = new int[width];
			for (int j = 0; j < width; ++j) {
				// initialize each cell in new grids to -1
				SquareCycles[i][j] = -1;
				ReducedGrid[i][j] = -1;
			}
		}

		// Fill SquareCycles Grid
		number = 0; // reusing the varible I used for this before
		for (int j = 0; j < height; j + 2) {
			for (int k = 0; k < width; k + 2) {
				SquareCycles[j][k] = number++;
				SquareCycles[j][k + 1] = number++;
				SquareCycles[j + 1][k + 1] = number++;
				SquareCycles[j + 1][k] = number++;
			}
		}

		// Find union of SquareCycles Grid and minDialated then put it in ReducedGrid
		Direction SquareEnter, SquareExit, DialatedEnter, DialatedExit, UnionEnter, UnionExit;
		int x1, x2 = 0, y1, y2 = 0;
		cur = 0;

		// Find next cells 
		for (cur = 0; cur < height * width; ++cur) {
			if (cur == height * width - 1) {
				cur = 0;
			}
			// Find next cell SquareCycles
			for (int j = 0; j < height; ++j) {
				for (int k = 0; k < width; ++k) {
					if (SquareCycles[j][k] == cur) {
						x1 = k;
						y1 = j;
					}
				}
			}
			// Find next cell minDialted
			if (minDilated[y2][x2] != cur) {
				if (DialatedExit == none || minDilated[y2][x2] == -1) {
					for (int j = 0; j < height; ++j) {
						for (int k = 0; k < width; ++k) {
							if (minDilated[j][k] == cur) {
								x2 = k;
								y2 = j;
								j = height;
								k = width;
							}
						}
					}
				}
				else {
					switch (DialatedExit){
						case up: --y2; break;
						case down: ++y2; break;
						case left: --x2; break;
						case right: ++x2; break;
						default: throw (1);
					}
				}
				GetDirection(minDilated, )
			}
		}

	}

// Given the grid "grid" has dimentions (height, width) this will find the 
// preceeding and next cells and store them in the variables dir1 for the 
// previous direction, and dir2 for the next direciton. If dir2 or dir2 are
// the return will represent if they are accurate
	bool GetDirection(int** grid, int y, int x, Direction& dir1, Direction& dir2, int height, int width, int max = 0) {
		Direction given1 = dir1, given2 = dir2;

		int x2, y2;
		int next = (grid[y][x] == max) ? 0 : grid[y][x] + 1;
		int previous = (grid[y][x] == 0) ? max : grid[y][x] - 1;

		if (y < height - 1 && grid[y + 1][x] == next) dir2 = down;
		else if (x > 0 && grid[y][x - 1] == next) dir2 = left;
		else if (y > 0 && grid[y - 1][x] == next) dir2 = up;
		else if (x < width - 1 && grid[y][x + 1] == next) dir2 = right;

		if (y < height - 1 && grid[y + 1][x] == previous) dir1 = up;
		else if (x > 0 && grid[y][x - 1] == previous) dir1 = right;
		else if (y > 0 && grid[y - 1][x] == previous) dir1 = down;
		else if (x < width - 1 && grid[y][x + 1] == previous) dir1 = left;
	
		if (given1 != none && given1 != dir1) return false;
		else if (given2 != none && given2 != dir2) return false;
		else return true;
	}
};

