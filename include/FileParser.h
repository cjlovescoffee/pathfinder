#pragma once

#include <string>
#include "MapData.h"

class FileParser
{
	public:
		void parseMapFileIntoMapData(const char& filename, MapData& outMapData) const;

	protected:
		std::string getFileExtension(const std::string& filename) const;
};