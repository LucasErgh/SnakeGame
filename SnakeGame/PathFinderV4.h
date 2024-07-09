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

		int** minDialated;
		int length = width;
		minDialated = new int* [length];
		for (int i = 0; i < length; ++length) {
			minDialated[i] = new int[length];
		}

		Direction enter;
		Direction exit;
		int Ox1, Oy1, Ox2, Oy2, Nx1, Ny1, Nx2, Ny2;
		int cur, number = 0;
		bool running = true;

		// Find starting point
		if (minimum[0][0] == 0) {
			Ox1 = 0;
			Oy1 = 0;
		}
		else {
			for (int j = 0; j < minLength; ++j) {
				for (int k = 0; k < minLength; ++k) {
					if (minimum[j][k] == 0) {
						Oy1 = j;
						Ox1 = k;
						break;
					}
				}
			}
		}

		// Place nodes on scaled version
		// Find previous direction
		cur = minimum[Oy1][Ox1];
		
		int previous;
		if (cur == 0) previous = minLength * minLength;
		else previous = --cur;

		if (minimum[Oy1 + 1][Ox1] == previous) enter = down;
		else if (minimum[Oy1][Ox1 + 1] == previous) enter = right;
		else if (minimum[Oy1 - 1][Ox1] == previous) enter = up;
		else if (minimum[Oy1][Ox1 - 1] == previous) enter = left;
		else throw(1);

		Ox2 = Ox1;
		Oy2 = Oy1;

		while (running) {
			// Loop to find the next turn
			do {
				int next;
				if (cur == minLength * minLength - 1) {
					next = 0;
				}
				else {
					next = cur + 1;
				}

				if (minimum[Oy2 + 1][Ox2] == next) 
				{ 
					exit = down; 
					Oy2 += 1;
				}
				else if (minimum[Oy2][Ox2 + 1] == next)
				{
					exit = right;
					Ox2 += 1;
				}
				else if (minimum[Oy2 - 1][Ox2] == next)
				{
					exit = up;
					Oy2 -= 1;
				}
				else if (minimum[Oy2][Ox2 - 1] == next)
				{
					exit = left;
					Ox2 -= 1;
				}
				else throw(1);
				++cur;

			} while (enter != exit);

			switch (exit)
			{
			case up:
				while (Ox1 != Ox2 && Oy1 != Oy2 + 1) {

				}
				break;
			case down:
				break;
			case left:
				break;
			case right:
				break;
			case none:
				break;
			default:
				break;
			}

			int newx = Ox2 * 2 + 1, newy = Oy2 * 2 + 1;

			if (enter == right || exit == right) {
				// if we went right in a turn we need to go up
				newy -= 1;
			}
			if (enter == down || exit == down) {
				// if we went down in a turn we need to go left
				newx -= 1;
			}



			// check if anything is out of bounds
			if (Oy2 < 0 || Ox2 < 0) throw (0);
		}
	}
};

