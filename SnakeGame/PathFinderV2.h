#pragma once

#include "PathFindingModel.h"

class Node {
public:
	Node(Snake* snake, Direction dir, int depth) : snake(snake), depth(depth), dir(dir), alive(true), cords(snake->headLocation()) { }
	
	Node* MakeChild(Direction dir) {
		// Checks if it has a child in the same direction
		for (auto cur : Children) {
			if (cur && cur->dir == dir)
				return NULL;
		}
		// Checks to see if new direction is opposite of old direction
		if (IsOposite(this->dir, dir)) {
			return NULL;
		}
		// Create info for child
		Node* Child = NULL;
		Snake* copy = snake->MakeCopy();
		copy->changeDirection(dir);
		copy->move();
		// Find open spot for child and assign child
		for(auto& cur : Children)
		{
			if (cur == NULL) {
				cur = new Node(copy, dir, depth + 1);
				Child = cur;
				if (copy->outOfBounds() || copy->snakeCollision()) {
					cur->alive = false;
					cur->snake->deleteNodes();
					delete cur->snake;
				}
				break;
			}
		}
		// If no node was created we delete the snake we made for it
		if (Child == NULL) {
			copy->deleteNodes();
			delete copy;
		}
		// If this node has all of its children we delete its snake to save mem
		bool hasAllChildren = true;
		for (auto cur : Children) {
			if (cur == NULL) {
				hasAllChildren = false;
			}
		}
		if (hasAllChildren) {
			this->snake->deleteNodes();
			this->snake = NULL;
		}

		if (Child->alive == false) return NULL;
		else return Child;
	}
	
	void operator~() {
		for (auto cur : Children) {
			if (cur) {
				delete cur;
				cur = NULL;
			}
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
		int searchDepth = 0; 
		int lastDeepestNode = 0;
		running = !TryShortestPath(root);
		while (running) {
			//this is where ill pathfind when shortest path doesn't work
			running = !GrowTree(root, ++searchDepth);
			int temp = DeepestNode(root)->depth;
			if (temp == lastDeepestNode) {
				return NULL;
			}
			lastDeepestNode = temp;
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
				if (list) {
					list->push_back(cur->dir);
					return list;
				}
			}
		}
		return NULL;
	}

	bool GrowTree(Node* node, int depth) {
		// This should never be hit
		if (!node)
			return false;
		// Checking if we are at search depth
		if (node->depth == depth) {
			if (node->cords == goal)
				return true;
			else return false;
		}
		// Growing tree for each direction
		for (int i = 0; i < 4; ++i) {
			if (IsOposite((Direction)i, node->dir))
				continue;
			
			// Checking to see if node already has a child in this direction
			bool hasDirection = false;
			for (auto cur : node->Children) {
				if (cur && cur->dir == (Direction)i) {
					hasDirection = true;
					break;
				}
			}
			if (hasDirection) continue;
			
			// Creating the node of this direction
			for (auto& cur : node->Children) {
				if (cur)
					continue;
				node->MakeChild((Direction)i);
				break;
			}
		}

		// Growing tree for each node and returning if any of them found the path
		for (auto& cur : node->Children) {
			if (cur && cur->alive) {
				if (GrowTree(cur, depth)) {
					return true;
				}
			}
		}
		return false;
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

		if (child && child->alive) {
			return (bool)TryShortestPath(child);
		}
		else return false;
	}
	Node* DeepestNode(Node* node) {
		Node* deepest = node;
		int deepestDepth = node->depth;
		Node* temp;
		for (auto cur : node->Children) {
			if (cur != NULL && cur->alive == true) {
				temp = DeepestNode(cur);
				if (temp->depth > deepestDepth) {
					deepest = temp;
					deepestDepth = deepest->depth;
				}
			}
		}
		return deepest;
	}
	void operator~() {
		DeleteChildren(root);
		delete root;
		root = NULL;
	}
	void DeleteChildren(Node* node) {
		for (auto& cur : node->Children) {
			DeleteChildren(cur);
			cur = NULL;
		}
		if (node->snake) {
			node->snake->deleteNodes();
			delete node->snake;
			node->snake = NULL;
		}
	}
private:
	Node* root;
	cords goal;
};

