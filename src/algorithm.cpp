#include "algorithm.hpp"

std::string trimBeginEndSpace(const std::string& str)
{
    size_t beg = 0;
    while (beg < str.size() && std::isspace(str[beg])) ++beg;
    size_t end = str.size()-1;
    while (end > beg && std::isspace(str[end])) --end;

    if (beg == end)
    {
        return "";
    }

    return str.substr(beg, end-beg+1);
}
