#pragma once
#include <string>
#include "MapData.h"

class MapSolver
{
	public:
		void getShortestPathToGoal(std::string& outResult, MapData& mapData, const bool& bAllowOrdinalMovement = false, const bool& bUseDijkstra = false) const;

	protected:
		void prepareMapData(MapData& mapData, const bool& bAllowOrdinalMovement = false) const;
		float distanceBetweenNodes(const MapData::sNode& i, const MapData::sNode& j) const;
};