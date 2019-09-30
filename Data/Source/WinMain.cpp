#include <XEngine.h>
#include <XMath.h>
#include "Graph.h"

const int columns = 32;
const int rows = 24;
const float cellSize = 32.0f;

int cursorTextureID = -1;

int StartTextureId = -1;
int EndTextureId = -1;

void DrawGrass(Coord ob)
{
	float radius = cellSize * 0.5f;
	X::DrawScreenCircle(X::Math::Vector2(ob.x * cellSize, ob.y * cellSize) + X::Math::Vector2::One() * radius, radius, X::Math::Vector4::Green());
}

void DrawLake(Coord ob)
{
	float radius = cellSize * 0.5f;
	X::DrawScreenCircle(X::Math::Vector2(ob.x * cellSize, ob.y * cellSize) + X::Math::Vector2::One() * radius, radius, X::Math::Vector4::Cyan());
}

void DrawGrid(int columns, int rows, int cellSize)
{
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < columns; ++x)
		{
			float left = x * cellSize;
			float top = y * cellSize;
			float right = (x + 1) * cellSize;
			float bottom = (y + 1) * cellSize;

			X::DrawScreenRect(left, top, right, bottom, X::Math::Vector4::DarkGray());
			if (G.GetNode(x, y)->t == terrainType::Grass)
			{
				Graph::Node* pos = G.GetNode(x, y);
				DrawGrass(pos->c);
			}
			if (G.GetNode(x, y)->t == terrainType::Lake)
			{
				Graph::Node* pos = G.GetNode(x, y);
				DrawLake(pos->c);
			}
		}
	}
}

void DrawCursor(int textureId)
{
	float mouseX = (float)X::GetMouseScreenX();
	float mouseY = (float)X::GetMouseScreenY();
	X::DrawSprite(textureId, X::Math::Vector2(mouseX, mouseY));
}

void DrawStart(Coord start)
{
	float radius = cellSize * 0.5f;
	X::DrawScreenCircle(X::Math::Vector2(start.x * cellSize, start.y * cellSize) + X::Math::Vector2::One() * radius, radius, X::Math::Vector4::Yellow());
}

void DrawEnd(Coord end)
{
	float radius = cellSize * 0.5f;
	X::DrawScreenCircle(X::Math::Vector2(end.x * cellSize, end.y * cellSize) + X::Math::Vector2::One() * radius, radius, X::Math::Vector4::Blue());
}

void DrawPath()
{
	for (int i = 0; i < closedList.size(); ++i)
	{
		if (closedList[i]->prevCoord != nullptr)
		{
			X::DrawScreenLine(X::Math::Vector2(closedList[i]->c.x + 0.5f, closedList[i]->c.y + 0.5f) * cellSize, X::Math::Vector2(closedList[i]->prevCoord->c.x + 0.5f, closedList[i]->prevCoord->c.y + 0.5f) * cellSize, X::Math::Vector4::Red());
		}
	}

	for (int i = 0; i < openList.size(); ++i)
	{
		if (openList[i]->prevCoord != nullptr)
		{
			X::DrawScreenLine(X::Math::Vector2(openList[i]->c.x + 0.5f, openList[i]->c.y + 0.5f) * cellSize, X::Math::Vector2(openList[i]->prevCoord->c.x + 0.5f, openList[i]->prevCoord->c.y + 0.5f) * cellSize, X::Math::Vector4::Blue());
		}
	}

	if (path.size() > 1)
	{
		for (int i = 0; i < path.size() - 1; ++i)
		{
			X::DrawScreenLine(X::Math::Vector2(path[i].x + 0.5f, path[i].y + 0.5f) * cellSize, X::Math::Vector2(path[i + 1].x + 0.5f, path[i + 1].y + 0.5f) * cellSize, X::Math::Vector4::Green());
		}
	}
}

std::vector<Coord> RunBFS(Graph& g, Coord s, Coord e)
{
	G.ResetNodes();
	openList.clear();
	closedList.clear();

	openList.push_back(G.GetNode(s));

	while (!openList.empty())
	{
		if (!closedList.empty() && closedList.back()->c == e)
		{
			break;
		}

		Graph::Node* next = openList.front();
		closedList.push_back(next);
		next->Closed();
		openList.pop_front();

		std::vector<Graph::Node*> neighbours = g.GetNeighbour(next);

		for (auto t : neighbours)
		{
			if (t->t == terrainType::Lake) continue;

			if (!t->open && !t->closed)
			{
				t->prevCoord = closedList.back();
				t->Open();
				openList.push_back(t);
			}
		}
	}

	std::vector<Coord> Path;
	if (closedList.back()->c == end)
	{
		Graph::Node* node = closedList.back();
		while (node != nullptr)
		{
			Path.push_back(node->c);
			node = node->prevCoord;
		}
	}
	return Path;
}

std::vector<Coord> RunDFS(Graph& g, Coord s, Coord e)
{
	G.ResetNodes();
	openList.clear();
	closedList.clear();

	openList.push_back(G.GetNode(s));

	while (!openList.empty())
	{
		if (!closedList.empty() && closedList.back()->c == e)
		{
			break;
		}

		Graph::Node* next = openList.front();
		closedList.push_back(next);
		next->Closed();
		openList.pop_front();

		std::vector<Graph::Node*> neighbours = g.GetNeighbour(next);

		for (auto t : neighbours)
		{
			if (t->t == terrainType::Lake) continue;

			if (!t->open && !t->closed)
			{
				t->prevCoord = closedList.back();
				t->Open();
				openList.push_front(t);
			}
		}
	}

	std::vector<Coord> Path;
	if (closedList.back()->c == end)
	{
		Graph::Node* node = closedList.back();
		while (node != nullptr)
		{
			Path.push_back(node->c);
			node = node->prevCoord;
		}
	}

	return Path;
}

std::vector<Coord> RunDij(Graph& g, Coord s, Coord e)
{
	G.ResetNodes();
	openList.clear();
	closedList.clear();

	openList.push_back(G.GetNode(s));

	while (!openList.empty())
	{
		if (!closedList.empty() && closedList.back()->c == e)
		{
			break;
		}

		Graph::Node* next = openList.back();
		closedList.push_back(next);
		next->Closed();
		openList.pop_back();

		std::vector<Graph::Node*> neighbours = g.GetNeighbour(next);

		for (auto t : neighbours)
		{
			if (t->t == terrainType::Lake)
			{
				continue;
			}

			if (t->open == true)
			{
				for (int i = 0; i < openList.size(); i++)
				{
					if (t == openList[i])
					{
						float d = G.GetDistance(t, next) + next->gCost;
						if (d < openList[i]->gCost)
						{
							openList.erase(openList.begin() + i);
							t->open = false;
						}
						break;
					}
				}
			}

			if (t->open == false && t->closed == false)
			{
				std::deque<Graph::Node*>::iterator open = openList.begin();
				float d = G.GetDistance(t, next) + next->gCost;
				while (open != openList.end() && d < (*open)->gCost)
				{
					open++;
				}
				t->prevCoord = next;
				t->gCost = d;
				t->Open();
				openList.insert(open, t);
			}
		}
	}

	std::vector<Coord> path;
	if (closedList.back()->c == e)
	{
		Graph::Node* node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(node->c);
			node = node->prevCoord;
		}
	}

	return path;
}

std::vector<Coord> RunAStar(Graph& g, Coord s, Coord e)
{
	G.ResetNodes();
	openList.clear();
	closedList.clear();

	Graph::Node* node = G.GetNode(s);
	node->hCost = G.EuclideanCost(s, e);
	openList.push_back(node);

	while (openList.size() > 0)
	{
		if (!closedList.empty() && closedList.back()->c == e)
		{
			break;
		}

		Graph::Node* next = openList.back();
		closedList.push_back(next);
		next->Closed();
		openList.pop_back();

		std::vector<Graph::Node*> neighbours = g.GetNeighbour(next);

		for (auto t : neighbours)
		{
			if (t->t == terrainType::Lake)
			{
				continue;
			}

			if (t->open == true)
			{
				for (int i = 0; i < openList.size(); i++)
				{
					if (t == openList[i])
					{
						float d = G.GetDistance(t, next) + next->gCost;
						if (d < openList[i]->gCost)
						{
							openList.erase(openList.begin() + i);
							t->open = false;
						}
						break;
					}
				}
			}

			if (t->open == false && t->closed == false)
			{
				std::deque<Graph::Node*>::iterator open = openList.begin();
				t->gCost = G.GetDistance(t, next) + next->gCost;
				t->hCost = G.EuclideanCost(t->c, e);
				while (open != openList.end() && t->gCost + t->hCost < (*open)->gCost + (*open)->hCost)
				{
					open++;
				}
				t->prevCoord = next;
				t->Open();
				openList.insert(open, t);
			}
		}
	}

	std::vector<Coord> path;
	if (closedList.back()->c == e)
	{
		Graph::Node* node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(node->c);
			node = node->prevCoord;
		}
	}

	return path;
}

bool GameLoop(float deltaTime)
{
	DrawGrid(columns, rows, cellSize);
	DrawCursor(cursorTextureID);
	DrawStart(start);
	DrawEnd(end);
	DrawPath();

	if (X::IsMousePressed(X::Mouse::LBUTTON))
	{
		start.x = X::Math::Min((int)(X::GetMouseScreenX() / cellSize), columns - 1);
		start.y = X::Math::Min((int)(X::GetMouseScreenY() / cellSize), rows - 1);
	}

	else if (X::IsMousePressed(X::Mouse::RBUTTON))
	{
		end.x = X::Math::Min((int)(X::GetMouseScreenX() / cellSize), columns - 1);
		end.y = X::Math::Min((int)(X::GetMouseScreenY() / cellSize), rows - 1);
	}
	else if (X::IsKeyDown(X::Keys::ONE))
	{
		Coord c;
		c.x = X::Math::Min((int)(X::GetMouseScreenX() / cellSize), columns - 1);
		c.y = X::Math::Min((int)(X::GetMouseScreenY() / cellSize), rows - 1);
		if (G.GetNode(c)->t != terrainType::Lake)
		{
			G.AddLake(c);
		}
	}
	else if (X::IsKeyDown(X::Keys::TWO))
	{
		Coord c;
		c.x = X::Math::Min((int)(X::GetMouseScreenX() / cellSize), columns - 1);
		c.y = X::Math::Min((int)(X::GetMouseScreenY() / cellSize), rows - 1);
		if (G.GetNode(c)->t != terrainType::Grass)
		{
			G.AddGrass(c);
		}
	}
	else if (X::IsKeyDown(X::Keys::THREE))
	{
		Coord c;
		c.x = X::Math::Min((int)(X::GetMouseScreenX() / cellSize), columns - 1);
		c.y = X::Math::Min((int)(X::GetMouseScreenY() / cellSize), rows - 1);
		G.ClearSpace(c);
	}
	if (X::IsKeyPressed(X::Keys::F1))
	{
		path = RunBFS(G, start, end);
	}
	if (X::IsKeyPressed(X::Keys::F2))
	{
		path = RunDFS(G, start, end);
	}
	if (X::IsKeyPressed(X::Keys::F3))
	{
		path = RunDij(G, start, end);
	}
	if (X::IsKeyPressed(X::Keys::F4))
	{
		path = RunAStar(G, start, end);
	}

	return X::IsKeyPressed(X::Keys::ESCAPE);
}
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	X::Start("Pathfinding", hInstance, 1024, 768);
	X::SetTexturePath("../Data/Images");
	cursorTextureID = X::LoadTexture("sword.png");
	G.Initialize(columns, rows);
	X::Run(GameLoop);
	X::Stop();
	return 0;
}
