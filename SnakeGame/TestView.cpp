#include "TestView.h"

TestView::TestView() {

}

void TestView::update(int height, int width, CellList board) {
	// Order List
	board.Order();

	// Draw top line
	for (int i = 0; i < (height + 2); ++i) {
		std::cout << '-';
	}
	std::cout << std::endl;

	// Draw rows
	bool didDraw = false;
	for (int row = height - 1; row >= 0; --row) {
		std::cout << '|';
		for (int colmn = 0; colmn < width; ++colmn) {
			didDraw = false;
			for (auto a : board.getList()) {
				if (a.first.first == colmn && a.first.second == row) {
					didDraw = true;
					switch (a.second)
					{
					case appleCell:
						std::cout << 'A';
						break;
					case bodyCell:
						std::cout << 'O';
						break;
					case headCell:
						std::cout << '0';
						break;
					default:
						break;
					}
				}
			}
			if (!didDraw) {
				std::cout << ' ';
			}
			/*if (cur < board.size() && board.x(cur) == colmn && board.y(cur) == row) {
				switch (board.type(cur))
				{
				case appleCell:
					std::cout << 'A';
					break;
				case bodyCell:
					std::cout << 'O';
					break;
				case headCell:
					std::cout << '0';
					break;
				default:
					break;
				}
				++cur;
			}*/
		}
		std::cout << '|' << std::endl;
		
	}


	// Draw bottem line
	for (int i = 0; i < (height + 2); ++i) {
		std::cout << '-';
	}
	std::cout << std::endl;

}
