#pragma once

#include <vector>

#include "Snake.h"
#include "PathFindingModel.h"

class SearchTreeNode : public PathFindingModel
{
	SearchTreeNode* Children[3] = { };

	Snake* snake;
	cords goal;
	Direction dir;
	bool FindGoal;
	bool SnakeDied;
	int depth;

	bool IsOposite(Direction dir1, Direction dir2) {
		switch (dir1)
		{
		case up:
			if (dir2 == down) return true;
			break;
		case down:
			if (dir2 == up) return true;
			break;
		case left:
			if (dir2 == right) return true;
			break;
		case right:
			if (dir2 == left) return true;
			break;
		}
		return false;
	}

	void PopulateChildren() {

		int cur = 0;
		for (int i = 0; i < 4; i++) {
			if (!IsOposite((Direction)i, dir)) {
				Children[cur++] = new SearchTreeNode(goal, snake->MakeCopy(), (Direction)i, depth + 1);
			}
		}
	}

	void PopulateLayer(int depth) {
		if (!SnakeDied) {
			if (this->depth < depth) {
				for (auto cur : Children) {
					cur->PopulateLayer(depth);
				}
			}
			else if (this->depth == depth){
				PopulateChildren();
			}
		}
	}
	std::vector<Direction>* SearchPath() {
		
		if (FindGoal == true) {
			std::vector<Direction>* directions = new std::vector<Direction>;
			directions->push_back(dir);
			return directions;
		}
		else if (SnakeDied == true) {
			return NULL;
		}
		else {
			std::vector<Direction>* directions = new std::vector<Direction>;
			for (auto cur : Children) {
				if (cur == NULL)
					continue;
				directions = cur->SearchPath();
				if (directions != NULL) {
					if (depth != 1)
						directions->push_back(dir);
					return directions;
				}
			}
			delete directions;
			return NULL;
		}
	}
public:
	SearchTreeNode(cords goal, Snake* snake, Direction dir, int depth = 1) : goal(goal), depth(depth), dir(dir), snake(snake), FindGoal(false), SnakeDied(false) {
		
		if (dir == none) {
			this->dir = snake->GetDirection();
		}
		else {
			snake->changeDirection(dir);
			if (snake->move() == goal)
				FindGoal = true;
			if (snake->outOfBounds() || snake->snakeCollision())
				SnakeDied = true;
		}
	}

	std::vector<Direction>* PathFindingModel::FindPath() {
		std::vector<Direction>* path = NULL;
		int searchDepth = 1;
		do {
			PopulateLayer(searchDepth++);
			path = SearchPath();
		} while (path == NULL);
		return path;
	}

	void PathFindingModel::Delete() {
		for (auto cur : Children) {
			if (cur != NULL) {
				cur->Delete();
				delete cur;
			}
		}
		snake->deleteNodes();
		delete snake;
	}
};

