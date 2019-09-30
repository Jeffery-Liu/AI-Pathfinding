#pragma once
#ifndef INCLUDED_XENGINE_GRAPH_H
#define INCLUDED_XENGINE_GRAPH_H

#include "XMath.h"
#include <list>
#include <queue>
#include <stack>
#include <vector>

enum terrainType
{
	Road = 0, 
	Grass = 1,
	Lake = 2
};

struct Coord
{
	Coord() {}
	Coord(int _x, int _y) : x(_x), y(_y) {}

	~Coord() {}

	bool operator==(Coord& rhs) { return x == rhs.x && y == rhs.y; }
	bool operator!=(Coord& rhs) { return x != rhs.x || y != rhs.y; }

	int x, y;
};
class Graph
{
public:
	struct Node
	{
		void Open()
		{
			open = true;
			closed = false;
		}

		void Closed()
		{
			closed = true;
			open = false;
		}

		Coord c;
		terrainType t;
		Node* prevCoord;
		bool open;
		bool closed;
		float gCost;
		float hCost;
	};
	Graph() : mNodes(nullptr), mRow(0), mColumn(0) {}
	~Graph() {}

	void Initialize(int rows, int columns)
	{
		mRow = rows;
		mColumn = columns;

		mNodes = new Node[rows * columns];
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; ++j)
			{
				mNodes[GetIndex(i, j)].c = Coord(i, j);
			}
		}
	}
	void Clear()
	{
		delete[] mNodes;
		mNodes = nullptr;

		mRow = 0;
		mColumn = 0;
	}

	inline int GetIndex(Coord c) const { return GetIndex(c.x, c.y); }
	inline int GetIndex(int row, int column) const { return row + column * mRow; }
	Node* GetNode(Coord c) const { return GetNode(c.x, c.y); }
	Node* GetNode(int x, int y) const { return &mNodes[GetIndex(x, y)]; }

	void AddGrass(Coord c) { mNodes[GetIndex(c)].t = terrainType::Grass; }
	void AddLake(Coord c) { mNodes[GetIndex(c)].t = terrainType::Lake; }
	void ClearSpace(Coord c) { mNodes[GetIndex(c)].t = terrainType::Road; }

	void ResetNodes() const
	{
		for (int i = 0; i < mRow * mColumn; ++i)
		{
			mNodes[i].closed = false;
			mNodes[i].open = false;
			mNodes[i].gCost = 0;
			mNodes[i].hCost = 0;
			mNodes[i].prevCoord = nullptr;
		}
	}
	std::vector<Node*> GetNeighbour(Node* c) const
	{
		std::vector<Node*> coords;

		for (int row = -1; row <= 1; ++row)
		{
			for (int column = -1; column <= 1; ++column)
			{
				// disable finding the way diagonally
				//if (row == 0 || column == 0)
				//{
					Coord coord(c->c.x + row, c->c.y + column);

					if (coord.x >= 0 && coord.y >= 0 && coord.x < mRow && coord.y < mColumn && c->c != coord)
					{
						coords.push_back(GetNode(coord));
					}
				//}
			}
		}

		return coords;
	}
	float GetDistance(Node* c1, Node* c2);
	float EuclideanCost(Coord c1, Coord c2);

	bool isValid(int x, int y) const { return isValid(Coord(x, y)); };
	bool isValid(Coord c) const { return (c.x >= 0 && c.y >= 0 && c.x < mRow && c.y < mColumn); }
private:
	Node* mNodes;
	unsigned int mRow;
	unsigned int mColumn;
};
Coord start;
Coord end;
Graph G;
std::vector<Coord> path;
std::deque<Graph::Node*>& openList = std::deque<Graph::Node*>();
std::deque<Graph::Node*>& closedList = std::deque<Graph::Node*>();

float Graph::GetDistance(Node* c1, Node* c2)
{
	float cost;
	if (c1->c.x == c2->c.x || c1->c.y == c2->c.y) cost = 1;
	else cost = X::Math::kRootTwo;

	if (c1->t == terrainType::Lake || c2->t == terrainType::Lake) cost *= 2;
	return cost;
}

float Graph::EuclideanCost(Coord c1, Coord end)
{
	return X::Math::Sqrt((end.x - c1.x)*(end.x - c1.x) + (end.y - c1.y)*(end.y - c1.y));
}

#endif // #ifndef INCLUDED_XENGINE_GRAPH_H