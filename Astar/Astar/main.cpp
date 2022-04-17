#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <List>
#include <windows.h>

using namespace std;


char map[10][10] = {
	{'S', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	{'1', '0', '1', '0', '1', '1', '1', '1', '1', '1'},
	{'1', '1', '1', '0', '1', '0', '0', '0', '1', '1'},
	{'1', '1', '0', '1', '1', '1', '1', '0', '1', '1'},
	{'1', '1', '1', '1', '1', '0', '1', '1', '1', '1'},
	{'1', '1', '0', '1', '1', '1', '1', '1', '0', '1'},
	{'1', '1', '1', '0', '1', '1', '1', '1', '1', '1'},
	{'1', '0', '1', '1', '0', '1', 'E', '1', '1', '1'},
	{'1', '1', '0', '1', '0', '1', '1', '0', '1', '1'},
	{'1', '1', '0', '1', '1', '1', '0', '1', '1', '1'},
};

class Pos
{
public:
	int x, y;

public:
	Pos(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};

class Node
{
public:
	Node* parent_Node;
	Node* childe_Node;
	Pos* pos;
	int g, h, f;

public:
	Node(int _x, int _y)
	{
		parent_Node = nullptr;
		childe_Node = nullptr;
		pos = new Pos(_x, _y);
		f = 0;
		g = 1;
		h = 0;
	}

};

list<Node*> openList;
list<Node*> closeList;

Pos* startPos;
Pos* endPos;

Node* StartNode;

void FindPath();
Pos* FindNodePos(char name);
void PrintMap(char color);
Node* CreateNode(Pos* _pos, Node* _parent, int _g);
Node* NewNode(Node* parent_Node, int x, int y);
void ShowPath();
void setColor(unsigned short text);

int main()
{
	startPos = FindNodePos('S');
	endPos = FindNodePos('E');

	PrintMap(15);

	StartNode = CreateNode(startPos, nullptr, 0);

	FindPath();

	return 0;
}

void FindPath()
{
	Node* currentNode = nullptr;
	int maxMapPos_X = sizeof(map) / sizeof(map[0]);
	int maxMapPos_Y = sizeof(map[0]);

	int cf = 10000;

	for (auto& iter : openList)
	{
		if (iter->f < cf)
		{
			cf = iter->f;
			currentNode = iter;
		}
	}

	if (currentNode != nullptr)
	{

		if (currentNode->pos->x + 1 < maxMapPos_X)
		{
			CreateNode(new Pos(currentNode->pos->x + 1, currentNode->pos->y), currentNode, currentNode->g + 10);
		}
		if (currentNode->pos->y + 1 < maxMapPos_Y)
		{
			CreateNode(new Pos(currentNode->pos->x, currentNode->pos->y + 1), currentNode, currentNode->g + 10);
		}
		if (currentNode->pos->x - 1 >= 0)
		{
			CreateNode(new Pos(currentNode->pos->x - 1, currentNode->pos->y), currentNode, currentNode->g + 10);
		}
		if (currentNode->pos->y - 1 >= 0)
		{
			CreateNode(new Pos(currentNode->pos->x, currentNode->pos->y - 1), currentNode, currentNode->g + 10);
		}

		openList.remove(currentNode);
		closeList.push_back(currentNode);

		if (map[currentNode->pos->x][currentNode->pos->y] != 'E')
		{
			FindPath();
		}
		else
		{
			while (currentNode != nullptr)
			{
				int x = currentNode->pos->x;
				int y = currentNode->pos->y;

				map[x][y] = '*';

				if (currentNode->parent_Node != nullptr) currentNode->parent_Node->childe_Node = currentNode;
				currentNode = currentNode->parent_Node;
			}

			ShowPath();

			PrintMap(15);
		}

	}
}

void PrintMap(char color)
{
	int maxMapPos_X = sizeof(map) / sizeof(map[0]);
	int maxMapPos_Y = sizeof(map[0]);

	cout << "\n=======================================" << endl;
	for (int i = 0; i < maxMapPos_X; i++)
	{
		for (int j = 0; j < maxMapPos_Y; j++)
		{
			setColor(color);

			if (startPos->x == i && startPos->y == j) setColor(12);
			else if (endPos->x == i && endPos->y == j) setColor(9);
			else if (map[i][j] == '*') setColor(14);

			cout << map[i][j];
		}
		cout << endl;
	}
	cout << "=======================================" << endl;
}

Pos* FindNodePos(char name)
{
	int all_x = sizeof(map) / sizeof(map[0]);
	int all_y = sizeof(map[0]);

	for (int x = 0; x < all_x; x++)
	{
		for (int y = 0; y < all_y; y++)
		{
			if (map[x][y] == name)
			{
				Pos* pos = new Pos(x, y);

				return pos;
			}
		}
	}

	return nullptr;
}

Node* CreateNode(Pos* _pos, Node* _parent, int _g)
{
	if (map[_pos->x][_pos->y] == '0') return nullptr;

	if (closeList.size() != 0)
	{
		for (auto& iter : closeList)
		{
			if (iter->pos->x == _pos->x && iter->pos->y == _pos->y)
			{
				return nullptr;
			}
		}
	}

	Node* node = NewNode(_parent, _pos->x, _pos->y);

	node->g = _g;
	node->h = abs(endPos->x - _pos->x) + abs(endPos->y - _pos->y);
	node->f = node->g + node->h;

	openList.push_back(node);

	return node;
}

Node* NewNode(Node* parent_Node, int x, int y)
{
	Node* node = new Node(x, y);
	node->parent_Node = parent_Node;

	return node;
}

void ShowPath()
{

	Node* node = StartNode;

	int n = 0;

	cout << "START => \n";

	while (node != nullptr)
	{
		int x = node->pos->x;
		int y = node->pos->y;

		cout << "map[" << x << "]" << "[" << y << "] => ";

		n++;

		if (n % 3 == 0) cout << endl;

		map[x][y] = '*';

		node = node->childe_Node;
	}

	cout << "END";

}

void setColor(unsigned short text) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}



