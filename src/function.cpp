#include "function.hpp"
#include "soundstream.hpp"

#include <sstream>
#include <string>

void playFunction(std::istream& input)
{
    std::string line;
    while (input.good())
    {
        std::getline(input, line);
        if (!line.size() || line[0] == '#') continue;
        std::stringstream reader(line);
    }
}
