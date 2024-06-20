#pragma once

#include "PathFindingModel.h"

class Node {
public:
	Node(Snake* snake, Direction dir, int depth) : snake(snake), depth(depth), dir(dir), alive(true) { }
	bool MakeChild(Direction dir) {
		if (IsOposite(this->dir, dir)) {
			return false;
		}
		Snake* copy = snake->MakeCopy();
		copy->changeDirection(dir);
		copy->move();
		for(auto cur : Children)
		{
			if (cur == NULL) {
				cur == new Node(copy, dir, depth + 1);
				if (copy->outOfBounds() || copy->snakeCollision()) {
					cur->alive == false;
				}
				return true;
			}
		}
		return false;
	}
	Node* Children[3];
	Snake* snake;
	Direction dir;
	int depth;
	bool alive;
};

class PathFinderV2 : public PathFindingModel
{
public:
	PathFinderV2(Snake* original, cords goal) : goal(goal) {
		Snake* rootSnake = original->MakeCopy();
		root = new Node(rootSnake, rootSnake->GetDirection(), 0);
	}
	std::vector<Direction>* PathFindingModel::FindPath() {
		bool running = true;
		while (running) {
			
		}
	}
	bool TryShortestPath(Node* cur) {
		cords curLocation = cur->snake->headLocation();
		Direction dir;
		if (curLocation.first > goal.first) {
			dir = left;
		}
		else if (curLocation.first < goal.first) {
			dir = right;
		}
		else if (curLocation.second > goal.second) {
			dir = down;
		}
		else if (curLocation.second < goal.second) {
			dir = up;
		}
		else if (curLocation.first == goal.first && curLocation.second == goal.second) {
			return true;
		}
		if (cur->MakeChild(dir)) {
			return TryShortestPath(cur);
		}
		else return false;
	}
	Node* DeepestNode(Node* node) {
		Node* deepest;
		int deepestDepth;
		while (true) {
			Node* temp;
			for (auto cur : node->Children) {
				if (cur != NULL) {
					temp = DeepestNode(cur);
					if (temp->depth > deepestDepth) {
						deepest = temp;
					}
				}
			}
		}
	}
private:
	Node* root;
	cords goal;
};

