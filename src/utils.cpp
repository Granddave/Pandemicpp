#include "utils.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

std::vector<std::string> split(const std::string& strToSplit, char delimeter)
{
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
        if (!item.empty())
        {
            splittedStrings.push_back(item);
        }
    }
    return splittedStrings;
}

std::string titleCase(std::string str, char delimiter)
{
    std::transform(
        str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
    std::replace(str.begin(), str.end(), delimiter, ' ');

    char last = ' ';
    std::for_each(str.begin(), str.end(), [&](char& c) {
        if (last == ' ' && c != ' ' && (::isalpha(c) != 0))
        {
            c = static_cast<char>(::toupper(c));
        }
        last = c;
    });

    return str;
}

std::string trim(const std::string& str, const std::string& whitespace)
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
    {
        return ""; // Empty line
    }

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::string readFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open() || !file.good())
    {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        assert(0);
        return std::string();
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}
