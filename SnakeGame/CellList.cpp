#include "CellList.h"
#include <vector>

void CellList::Load(cords apple, cords head, std::vector<cords> body) {
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
		while ((i + 1 < list.size()) ||
			(list[i].first.first < list[i + 1].first.first) ||
			((list[i].first.first == list[i + 1].first.first) &&
			(list[i].first.second < list[i + 1].first.second)))
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