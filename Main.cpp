#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "FileParser.h"
#include "MapData.h"
#include "MapSolver.h"

int main(int argc, const char * argv[])
{
    try
    {
        if (argc < 2)
        {
            throw std::invalid_argument("Expected filename parameter");
        }

        const char* filename = argv[1];

        bool bUseDijkstra = false;

        if (argc > 2)
        {
            if (strcmp(argv[3], "dijkstra") == 0)
            {
                bUseDijkstra = true;
            }
        }

        FileParser* fileParser = new FileParser();
        MapData* mapData = new MapData();

        fileParser->parseMapFileIntoMapData(*filename, *mapData);

        delete fileParser;

        std::string* result = new std::string;
        MapSolver* mapSolver = new MapSolver();
        
        mapSolver->getShortestPathToGoal(*result, *mapData, false, bUseDijkstra);
        
        delete mapSolver;
        delete mapData;

        std::cout << "Shortest path to exit: " << *result << std::endl;

        delete result;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    exit(0);
}