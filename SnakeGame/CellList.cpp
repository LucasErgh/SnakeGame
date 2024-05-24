#include "CellList.h"
#include <vector>

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