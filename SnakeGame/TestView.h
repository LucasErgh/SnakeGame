#include "View.h" 
#include <iostream>

#pragma once
class TestView : public View {
	void update(int height, int width, cells board) {
		// Draw top line
		for (int i = 0; i <= (height + 2); ++i) {
			std::cout << '-';
		}
		std::cout << std::endl;

		// Draw rows
		std::cout << '|';


		// Draw bottem line
	}
};
