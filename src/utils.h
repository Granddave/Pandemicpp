#pragma once

#include <algorithm>
#include <random>
#include <string>
#include <vector>

#define UNUSED(x) (void)(x)

template <typename Iterator>
void shuffle(Iterator first, Iterator last)
{
    std::shuffle(first, last, std::mt19937(std::random_device()()));
}

std::vector<std::string> split(const std::string& strToSplit, char delimeter);
std::string titleCase(std::string str, char delimiter = '_');
std::string trim(const std::string& str, const std::string& whitespace = " \t");
std::string readFile(const std::string& filepath);
