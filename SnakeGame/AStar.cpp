#include "AStar.h"

Node::Node(cords cords, Node* parent, Direction direction) {
	pos = cords;
	this->parent = parent;
	G = H = 0;
	dir = direction;
}

int Node::GetScore()
{
	return G + H;
}

bool AStar::collision(cords check, int movement) {
	if (check.first < 1 || check.first > width || check.second < 1 || check.second > height)
		return true;
	for (int i = 0; i < walls.size(); ++i) {
		if (walls[i] == check && (size - i) > movement) {
			return true;
		}
	}


	return false;
}

void AStar::Shift(Direction dir, cords& cur) {
	switch (dir)
	{
	case up: --cur.second; return;
	case down: ++cur.second; return;
	case left: --cur.first; return;
	case right: ++cur.first; return;
	default: throw(1);
	}
}

Node* AStar::findNode(NodeSet& nodes, cords cur) {
	for (auto node : nodes) {
		if (node->pos == cur) {
			return node;
		}
	}
	return NULL;
}

int AStar::distance(cords cords1, cords cords2) {
	int total = 0;

	if (cords1.first < cords2.first)
		total += cords2.first - cords1.first;
	else if (cords1.first > cords2.first)
		total += cords1.first - cords2.first;

	if (cords1.second < cords2.second)
		total += cords2.second - cords1.second;
	else if (cords1.second > cords2.second)
		total += cords1.second - cords2.second;

	return total;
}

std::vector<Direction>* AStar::getDirections(CordList cordList) {
	std::vector<Direction>* directions = new std::vector<Direction>;

	for (int i = 0; i < cordList.size() - 1; ++i) {
		cords cur = cordList[i];
		cords next = cordList[i + 1];

		if (cur.second > next.second) directions->push_back(down);
		else if (cur.second < next.second) directions->push_back(up);
		else if (cur.first < next.first) directions->push_back(left);
		else if (cur.first > next.first) directions->push_back(right);
	}
	return directions;
}

AStar::AStar(Snake* snake) {
	if (snake) {
		updateSnake(snake);
	}
}

void AStar::updateSnake(Snake* snake) {
	walls.reserve(1);
	walls = snake->bodyLocation();

	start = snake->headLocation();
	startDir = snake->getDirection();
	height = snake->height;
	width = snake->width;
	size = snake->getSize();
}

std::vector<Direction>* AStar::FindPath(cords goal) {
	Node* current = NULL;
	NodeSet openSet; // This is for nodes who have not been checked
	NodeSet	closedSet; // This is for nodes who have been checked

	openSet.push_back(new Node(start, NULL, startDir));

	while (!openSet.empty()) {
		auto root = openSet.begin();
		current = *root;

		// Find best node as of now
		for (auto cur = openSet.begin(); cur != openSet.end(); ++cur) {
			auto node = *cur;
			if (node->GetScore() <= current->GetScore()) {
				current = node;
				root = cur;
			}
		}

		// Check if path was found
		if (current->pos == goal) {
			break;
		}

		//  
		closedSet.push_back(current);
		openSet.erase(root);

		// Grow graph from new best cell
		for (int i = 0; i < 4; ++i) {
			// make new cell in correct direction
			if (IsOposite((Direction)i, current->dir)) continue;
			
			cords newCords = current->pos;
			Shift((Direction)i, newCords);
			if (collision(newCords, current->G + 1) || findNode(closedSet, newCords)) {
				continue;
			}

			int totalCost = current->G + 1;

			Node* successor = findNode(openSet, newCords);
			if (!successor) {
				successor = new Node(newCords, current, (Direction)i);
				successor->G = totalCost;
				successor->H = distance(successor->pos, goal);
				openSet.push_back(successor);
			}
			else if (totalCost < successor->G) {
				successor->parent = current;
				successor->G = totalCost;
			}
		}
	}

	CordList path;
	while (current != NULL) {
		path.push_back(current->pos);
		current = current->parent;
	}

	for (auto cur : openSet) {
		delete cur;
	}
	for (auto cur : closedSet) {
		delete cur;
	}

	return getDirections(path);
}
