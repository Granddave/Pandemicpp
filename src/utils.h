#pragma once

#include <vector>
#include <string>

#define UNUSED(x) (void)(x)

std::vector<std::string> split(const std::string& strToSplit, char delimeter);
std::string titleCase(std::string str, char delimiter = '_');
std::string trim(const std::string& str, const std::string& whitespace = " \t");
std::string readFile(const std::string& filepath);
