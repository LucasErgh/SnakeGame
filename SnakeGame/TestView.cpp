#include "TestView.h"

#include "DataTypes.h"
#include <vector>

class CellList
{
public:
	void Load(cords apple, cords head, std::vector<cords> body);

	void Order();

	std::vector<cell> getList();

	int x(int index);

	int y(int index);

	CellType type(int index);

	int size();

private:
	std::vector<cell> list;
};


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

void CellList::Load(cords apple, cords head, std::vector<cords> body) {
	list.clear();
	list.push_back(std::make_pair(apple, appleCell));
	list.push_back(std::make_pair(head, headCell));
	for (auto cur : body) {
		list.push_back(std::make_pair(cur, bodyCell));
	}
}

void CellList::Order() {
	int i;
	bool didSwap = true;
	// this is a bubble sort that sorts in accending order first by x then y
	while (didSwap) {
		didSwap = false;
		i = 0;
		while (i + 1 < list.size() && (list[i].first.second < list[i + 1].first.second || ((list[i].first.second == list[i + 1].first.second &&
			(list[i].first.first > list[i + 1].first.first)))))
		{
			std::swap(list[i], list[i + 1]);
			didSwap = true;
			++i;
		}
	}
}

std::vector<cell> CellList::getList() {
	return list;
}

int CellList::x(int index) {
	return list[index].first.first;
}

int CellList::y(int index) {
	return list[index].first.second;
}

CellType CellList::type(int index) {
	return list[index].second;
}

int CellList::size() {
	return list.size();
}