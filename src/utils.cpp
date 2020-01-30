#include "utils.h"

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<std::string> split(const std::string& strToSplit, char delimeter)
{
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
        splittedStrings.push_back(item);
    }
    return splittedStrings;
}

std::string titleCase(std::string str, char delimiter)
{
    std::replace(str.begin(), str.end(), delimiter, ' ');

    char last = ' ';
    std::for_each(str.begin(), str.end(), [&](char& c) {
        if(last == ' ' && c != ' ' && ::isalpha(c))
            c = static_cast<char>(::toupper(c));
        last = c;
    });

    return str;
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
