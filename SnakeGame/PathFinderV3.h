#pragma once
#include "PathFindingModel.h"
/*
This was going to be a version of the a* method adapted to a grid rather than a graph but 
I realized this method won't work because the different paths to the apple can't be reduced 
to how many moves it took to get there because the snake will move in different ways changing the obsticle
*/

class Node {
public:
	Node(bool obstical, cords cords, int movementCost, int distanceCost, int sumCost, int age = 0)
		: obstical(obstical), cords(cords), age(age), movementCost(movementCost), distanceCost(distanceCost), sumCost(distanceCost + movementCost)
	{}

	cords cords;
	
	bool obstical;
	int age;
	
	int movementCost;
	int distanceCost;
	int sumCost;
};

class PathFinderV3 : public PathFindingModel
{
public:
	PathFinderV3(Snake *snake, int height, int width) {
		grid = new Node**[width];
		for (int i = 0; i < width; ++i) {
			grid[i] = new Node*[height];
		}
		int i = snake->size - 1;
		for (auto cur : snake->bodyLocation()) {
			grid[cur.first][cur.second] = new Node(true, cur, 0, 0, 0, i--);
		}
		grid[snake->headLocation().first][snake->headLocation().second] = new Node(true, snake->headLocation(), 0, 0, 0, snake->size);
	}
	void PathFindingModel::Delete() {

	}
	std::vector<Direction>* PathFindingModel::FindPath() {

	}
private:
	Node*** grid;
};

