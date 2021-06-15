#include "MapData.h"
#include <algorithm>

MapData::~MapData()
{
    for (std::vector<MapData::sNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        delete (*it);
    }
}

MapData::sNode& MapData::addNode(const char& character, const int& x, const int& y)
{
    MapData::sNode* node = new MapData::sNode;

    this->width = std::max(this->width, x);     // Width is equal to the highest x value provided to MapData
    this->height = std::max(this->height, y);   // Height is equal to the highest y value provided to MapData

    node->x = x;
    node->y = y;
    node->character = character;

    switch (character)
    {
        case 'A':
            this->start = node;
            break;
        case 'B':
            this->goal = node;
            break;
        case '.':
            break;
        case 'x':
        default:
            node->bIsTraversable = false;
            break;
    }

    this->nodes.emplace_back(node);

    return *node;
}

MapData::sNode* MapData::getStart() const
{
    return this->start;
}

MapData::sNode* MapData::getGoal() const
{
    return this->goal;
}

std::vector<MapData::sNode*>& MapData::getNodes()
{
    return this->nodes;
}

const int MapData::getHeight() const
{
    return this->height;
}

const int MapData::getWidth() const
{
    return this->width;
}