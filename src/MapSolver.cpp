#include "MapSolver.h"
#include <stdexcept>
#include <list>

void MapSolver::getShortestPathToGoal(std::string& outResult, MapData& mapData, const bool& bAllowOrdinalMovement, const bool& bUseDijkstra) const
{
	MapData::sNode* start = mapData.getStart();
	MapData::sNode* goal = mapData.getGoal();

	if (start == nullptr)
	{
		throw std::invalid_argument("The provided map does not contain a valid starting point");
	}

	if (goal == nullptr)
	{
		throw std::invalid_argument("The provided map does not contain a valid goal");
	}

	this->prepareMapData(mapData, bAllowOrdinalMovement);

	start->fDistanceFromStart = 0.0f;
	start->fDistanceFromGoal = bUseDijkstra ? 1.0f : distanceBetweenNodes(*start, *goal);

	MapData::sNode* current = start;

	// The 'stack' is maintained by the algorithm
	std::list<MapData::sNode*> stack;

	// ...but to kick things off we manually add our start point
	stack.emplace_back(start);

	// The algorithm continues to run while there are nodes in the stack
	// The algorithm will stop when it reaches the goal
	while (!stack.empty() && current != goal)
	{
		// Sort the stack by each node's distance from the goal from closest to furthest away
		stack.sort([](const MapData::sNode* left, const MapData::sNode* right) {
			return left->fDistanceFromGoal < right->fDistanceFromGoal;
		});

		// Remove any nodes that have already been visisted from the stack so that they do not get processed more than once
		while (!stack.empty() && stack.front()->bVisited)
		{
			stack.pop_front();
		}

		// If the stack is empty, we have visited all necessary nodes and can break out
		if (stack.empty())
		{
			break;
		}

		current = stack.front();
		current->bVisited = true;

		for (std::vector<MapData::sNode*>::iterator it = current->neighbouringNodes.begin(); it != current->neighbouringNodes.end(); it++)
		{
			// For each of the current nodes neighbours, if it has not yet been visited and is traversable, add it to the stack
			if (!(*it)->bVisited && (*it)->bIsTraversable)
			{
				stack.emplace_back(*it);
			}

			float fDistanceFromStartViaCurrent = current->fDistanceFromStart + distanceBetweenNodes(*current, *(*it));

			// If the current nodes provides this neighbour with a more optimal route back to the start, update the neighbour to reflect that
			if (fDistanceFromStartViaCurrent < (*it)->fDistanceFromStart)
			{
				(*it)->parent = current;
				(*it)->fDistanceFromStart = fDistanceFromStartViaCurrent;
				(*it)->fDistanceFromGoal = (*it)->fDistanceFromStart + (bUseDijkstra ? 1.0f : distanceBetweenNodes(*(*it), *goal));
			}
		}
	}

	if (goal->parent == nullptr)
	{
		throw std::invalid_argument("No valid path to goal");
	}
	
	current = goal;

	while (current->parent != nullptr)
	{
		if (current->x == current->parent->x)
		{
			if (current->y < current->parent->y)
			{
				outResult.insert(0, "N");
			}
			else if (current->y > current->parent->y)
			{
				outResult.insert(0, "S");
			}
		}
		else if (current->y == current->parent->y)
		{
			if (current->x < current->parent->x)
			{
				outResult.insert(0, "W");
			}
			else if (current->x > current->parent->x)
			{
				outResult.insert(0, "E");
			}
		}

		current = current->parent;
	}
}

float MapSolver::distanceBetweenNodes(const MapData::sNode& i, const MapData::sNode& j) const
{
	float a = (float) i.x - j.x;
	float b = (float) i.y - j.y;
	return sqrtf((a * a) + (b * b)); // Pythagorean theorum - we assume that the nodes are spaced apart equally
}

void MapSolver::prepareMapData(MapData& mapData, const bool& bAllowOrdinalMovement) const
{
	std::vector<MapData::sNode*> nodes = mapData.getNodes();

	int height = mapData.getHeight();
	int width = mapData.getWidth();

	// Creating an array of nodes with reliable indexes (based on their position according to values of x and y) means that we can treat gaps in irregularly-formed maps as non-traversable nodes
	int numberOfNodes = ((width + 1) * (height + 1));

	MapData::sNode** nodeArray = new MapData::sNode * [numberOfNodes]();

	for (std::vector<MapData::sNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		(*it)->bVisited = false;
		(*it)->parent = nullptr;
		(*it)->neighbouringNodes.clear();
		int index = ((*it)->y * (width + 1)) + (*it)->x; // Calculate a reliable index for the node
		nodeArray[index] = (*it);
	}

	for (int n = 0; n < numberOfNodes; n++)
	{
		MapData::sNode* node = nodeArray[n];

		if (node != nullptr)
		{
			if (node->y > 0)
			{
				MapData::sNode* northernNeighbour = nodeArray[n - width - 1];

				if (northernNeighbour != nullptr && northernNeighbour->bIsTraversable)
				{
					node->neighbouringNodes.emplace_back(northernNeighbour);
				}
			}

			if (node->x > 0)
			{
				MapData::sNode* easternNeighbour = nodeArray[n - 1];

				if (easternNeighbour != nullptr && easternNeighbour->bIsTraversable)
				{
					node->neighbouringNodes.emplace_back(easternNeighbour);
				}
			}

			if (node->y < height)
			{
				MapData::sNode* southernNeighbour = nodeArray[n + width + 1];

				if (southernNeighbour != nullptr && southernNeighbour->bIsTraversable)
				{
					node->neighbouringNodes.emplace_back(southernNeighbour);
				}
			}

			if (node->x < width)
			{
				MapData::sNode* westernNeighbour = nodeArray[n + 1];

				if (westernNeighbour != nullptr && westernNeighbour->bIsTraversable)
				{
					node->neighbouringNodes.emplace_back(westernNeighbour);
				}
			}

			if (bAllowOrdinalMovement)
			{
				// TODO implement NE, NW, SE, SW
			}
		}
	}

	delete[] nodeArray;
}