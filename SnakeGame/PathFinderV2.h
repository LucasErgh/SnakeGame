#pragma once

#include "PathFindingModel.h"

class Node {
public:
	Node(Snake* snake, Direction dir, int depth) : snake(snake), depth(depth), dir(dir), alive(true), cords(snake->headLocation()) { }
	
	Node* MakeChild(Direction dir) {
		bool hasAllChildren = true;
		Node* Child = NULL;
		if (IsOposite(this->dir, dir)) {
			return NULL;
		}
		Snake* copy = snake->MakeCopy();
		copy->changeDirection(dir);
		copy->move();
		for(auto& cur : Children)
		{
			if (!hasAllChildren && cur == NULL) {
				break;
			}
			else if (cur == NULL) {
				cur = new Node(copy, dir, depth + 1);
				Child = cur;
				if (copy->outOfBounds() || copy->snakeCollision()) {
					(*cur).alive == false;
					Child->snake->deleteNodes();
					delete Child->snake;
					Child = NULL;
				}
				hasAllChildren = false;
			}
		}
		if (hasAllChildren) {
			this->snake->deleteNodes();
			this->snake = NULL;
		}
		return Child;
	}
	
	void operator~() {
		for (auto cur : Children) {
			delete cur;
			cur = NULL;
		}
		if (snake) {
			snake->deleteNodes();
			delete snake;
			snake = NULL;
		}
	}

	Node* Children[3] = { NULL };
	Snake* snake;
	Direction dir;
	cords cords;
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
		running = !TryShortestPath(root);
		while (running) {
			//this is where ill pathfind when shortest path doesn't work
			running = false;
		}
		return (GetPath(root));
	}

	std::vector<Direction>* GetPath(Node* newRoot) {
		bool hasChildren = false;
		std::vector<Direction>* list;
		
		if (!newRoot) {
			return NULL;
		}

		if (newRoot->cords == goal) {
			list = new std::vector<Direction>();
			list->push_back(newRoot->dir);
			return list;
		}
		
		for (auto cur : newRoot->Children) {
			{
				list = GetPath(cur);
				if (cur) {
					list->push_back(cur->dir);
					return list;
				}
			}
		}
		return NULL;
	}

	bool TryShortestPath(Node* cur) {
		cords curLocation = cur->cords;
		Direction dir;
		if (curLocation.first > goal.first) {
			dir = left;
		}
		else if (curLocation.first < goal.first) {
			dir = right;
		}
		else if (curLocation.second > goal.second) {
			dir = up;
		}
		else if (curLocation.second < goal.second) {
			dir = down;
		}
		else if (curLocation.first == goal.first && curLocation.second == goal.second) {
			return true;
		}

		Node* child = cur->MakeChild(dir);

		if (child) {
			return (bool)TryShortestPath(child);
		}
		else return false;
	}
	Node* DeepestNode(Node* node) {
		Node* deepest = NULL;
		int deepestDepth = node->depth;
		while (true) {
			Node* temp ;
			for (auto cur : node->Children) {
				if (cur != NULL && cur->alive == true) {
					temp = DeepestNode(cur);
					if (temp->depth > deepestDepth) {
						deepest = temp;
					}
				}
			}
		}
		return deepest;
	}
	void operator~() {
		delete root;
		root = NULL;
	}
private:
	Node* root;
	cords goal;
};

