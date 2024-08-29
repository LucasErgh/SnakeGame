#include "HamCycle.h"

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
	int** minimum = new int* [minLength];
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
	int DilatedMax;
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
			int next = (val >= minLength * minLength - 1) ? val - (minLength * minLength - 1) : val + 1;

			if (!GetDirection(minimum, Oy2, Ox2, &enter, &exit, minLength, minLength, minLength * minLength - 1)) {
				throw(1); // If enterance isn't correct throw
			}

			// move to the next cell to check for turn
			Shift(exit, Oy2, Ox2);

			++val;
			if (exit == none) {
				throw(1);
			}
		} while (enter == exit);

		Nx2 = Ox2;
		Ny2 = Oy2;

		// undo last move
		Shift(Oposite(exit), Ny2, Nx2);

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

			Shift(enter, Ny1, Nx1);
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
			DilatedMax = number - 1;
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
	int** SquareCycles = new int* [height];
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
	number = 0;
	for (int j = 0; j < height; j += 2) {
		for (int k = 0; k < width; k += 2) {
			SquareCycles[j][k] = number++;
			SquareCycles[j][k + 1] = number++;
			SquareCycles[j + 1][k + 1] = number++;
			SquareCycles[j + 1][k] = number++;
		}
	}

	// Find the union of SquareCycles Grid and minDialated then put it in ReducedGrid
	// (I beleive its actually not the union, but rather, the difference of their union and their intersection but Ill be saying union)
	Direction SquareEnter = none, SquareExit = none, DialatedEnter = none, DialatedExit = none, UnionEnter = up, UnionExit = none;
	int y1 = 0, x1 = 0, // these keep track of our position in the ReducedGrid
		y2 = 0, x2 = 0; // these keep track of the next turn in the ReducedGrid
	int curUnion = 0;


	// Find next cells 
	while (curUnion < height * width) {
		if (curUnion == height * width) {
			throw(1);
		}

		// This loop find the next turn
		do {
			// We need to find the min and max of the small cycle to get the direction in the SquareGrid
			int tempx, tempy;

			if (x2 % 2 == 0) tempx = x2;
			else			 tempx = x2 - 1;

			if (y2 % 2 == 0) tempy = y2;
			else			 tempy = y2 - 1;

			GetDirection(SquareCycles, y2, x2, &SquareEnter, &SquareExit, height, width, SquareCycles[tempy + 1][tempx], SquareCycles[tempy][tempx]);

			// switch direction of enterances
			Direction OpositeEnter = Oposite(UnionEnter);
			SquareEnter = Oposite(SquareEnter);

			// if the dialated grid isn't filled in here we just go with the square grid
			if (minDilated[y2][x2] == -1) {
				if (OpositeEnter != SquareExit)
					UnionExit = SquareExit;
				else
					UnionExit = SquareEnter;
			}
			// otherwise we have to find the union
			else {
				// now find the direction of the current cel in minDialated and SquareCycles
				if (!GetDirection(minDilated, y2, x2, &DialatedEnter, &DialatedExit, height, width, DilatedMax)) {
					throw(1);
				}

				// swap the enterances because those are in the oposite direction
				DialatedEnter = Oposite(DialatedEnter);

				// Now we find the correct direction
				if (DialatedEnter != none && DialatedEnter != SquareEnter && DialatedEnter != SquareExit && DialatedEnter != OpositeEnter) {
					UnionExit = DialatedEnter;
				}
				else if (DialatedExit != none && DialatedExit != SquareEnter && DialatedExit != SquareExit && DialatedExit != OpositeEnter) {
					UnionExit = DialatedExit;
				}
				else if (SquareEnter != none && SquareEnter != DialatedEnter && SquareEnter != DialatedExit && SquareEnter != OpositeEnter) {
					UnionExit = SquareEnter;
				}
				else if (SquareExit != none && SquareExit != DialatedEnter && SquareExit != DialatedExit && SquareExit != OpositeEnter) {
					UnionExit = SquareExit;
				}
				else throw(1);
			}

			// move to next cell to check for turn
			Shift(UnionExit, y2, x2);

		} while (UnionEnter == UnionExit);

		int BSy = y2, BSx = x2; // positions before they were shifted back

		// undo last move
		Shift(Oposite(UnionExit), y2, x2);

		// Mark points in ReducedGrid
		if (curUnion == 0) {
			// this will set the both corners to the same value skiping the next loop
			ReducedGrid[y1][x1] = curUnion++;
			x1 = x2;
			y1 = y2;
		}
		else {
			// fill in points between corners
			while ((x1 != x2 || y1 != y2)) {
				if (x1 == 0 && y1 == 0)
					break;
				ReducedGrid[y1][x1] = curUnion++;
				Shift(UnionEnter, y1, x1);
			}
			// fill corner
			if (x1 == 0 && y1 == 0)
				break;
			ReducedGrid[y2][x2] = curUnion++;
		}

		// set starting points to new corner
		x2 = BSx;
		y2 = BSy;
		x1 = x2;
		y1 = y2;
		UnionEnter = UnionExit;
		UnionExit = none;
	}
	for (int i = 0; i < height; ++i) {
		delete[] SquareCycles[i];
		delete[] minDilated[i];
	}
	delete[] minDilated;
	delete[] SquareCycles;
	return ReducedGrid;
}

bool GetDirection(int** grid, int y, int x, Direction* dir1, Direction* dir2, int height, int width, int max, int min = 0) {
	int x2, y2;
	int next = (grid[y][x] == max) ? min : grid[y][x] + 1;
	int previous = (grid[y][x] == min) ? max : grid[y][x] - 1;

	if (y < height - 1 && grid[y + 1][x] == next) *dir2 = down;
	else if (x > 0 && grid[y][x - 1] == next) *dir2 = left;
	else if (y > 0 && grid[y - 1][x] == next) *dir2 = up;
	else if (x < width - 1 && grid[y][x + 1] == next) *dir2 = right;
	else return false;

	if (dir1 == NULL) return true;
	if (y < height - 1 && grid[y + 1][x] == previous) *dir1 = up;
	else if (x > 0 && grid[y][x - 1] == previous) *dir1 = right;
	else if (y > 0 && grid[y - 1][x] == previous) *dir1 = down;
	else if (x < width - 1 && grid[y][x + 1] == previous) *dir1 = left;
	else return false;

	return true;
}

void Shift(Direction dir, int& y, int& x) {
	switch (dir)
	{
	case up: --y; return;
	case down: ++y; return;
	case left: --x; return;
	case right: ++x; return;
	default: throw(1);
	}
}