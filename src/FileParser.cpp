#include <fstream>
#include <stdexcept>
#include "FileParser.h"

void FileParser::parseMapFileIntoMapData(const char& filename, MapData& outMapData) const
{
    std::string extension = getFileExtension(&filename);

    if (extension != "txt")
    {
        throw std::invalid_argument("Expected file with extension '.txt'");
    }

    std::ifstream file(&filename);

    if (file.fail())
    {
        throw std::invalid_argument("File does not exist");
    }

    if (file.is_open())
    {
        std::string line;
        int y = 0;

        while (std::getline(file, line))
        {
            for (int x = 0; x < line.length(); x++)
            {
                outMapData.addNode(line[x], x, y);
            }

            y++;
        }

        file.close();
    }
}

std::string FileParser::getFileExtension(const std::string& filename) const
{
    size_t indexOfLastDot = filename.find_last_of(".");

    if (indexOfLastDot != std::string::npos)
    {
        return filename.substr(indexOfLastDot + 1);
    }
        
    return "";
}