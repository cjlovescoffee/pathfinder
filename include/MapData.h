#pragma once
#include <vector>

class MapData
{
	public:
		struct sNode
		{
			char character;							// The original character that represents this node
			int x;									// Position on the x-axis
			int y;									// Position on the y-axis
			std::vector<sNode*> neighbouringNodes;  // Nodes that this node could travel into

			bool bIsTraversable = true;				// Whether or not this node is traversable
			bool bVisited = false;					// Whether or not this node has been visited before
			float fDistanceFromGoal = INFINITY;		// Distance from goal
			float fDistanceFromStart = INFINITY;	// Distance from start
			sNode* parent = nullptr;				// The node that offers the shortest path to the start
		};

		~MapData();

		sNode& addNode(const char& character, const int& x, const int& y);
		sNode* getStart() const;
		sNode* getGoal() const;
		std::vector<sNode*>& getNodes();
		const int getHeight() const;
		const int getWidth() const;

	protected:
		std::vector<sNode*> nodes;
		sNode* start;
		sNode* goal;
		int height = 0;
		int width = 0;
};

