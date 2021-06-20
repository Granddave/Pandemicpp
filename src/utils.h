#pragma once

#include <algorithm>
#include <cassert>
#include <random>
#include <string>
#include <vector>

#include "logging.h"

#define UNUSED(x) (void)(x)

#define VERIFY(expr) assert(expr);

#define VERIFY_LOG(expr, ...)  \
    if (!(expr))               \
    {                          \
        LOG_CRIT(__VA_ARGS__); \
        VERIFY(expr);          \
    }

#define VERIFY_NOT_REACHED() VERIFY(false)
#define TODO() VERIFY(false)

template <typename Iterator>
void shuffle(Iterator first, Iterator last)
{
    std::shuffle(first, last, std::mt19937(std::random_device()()));
}

std::vector<std::string> split(const std::string& strToSplit, char delimeter);
std::string titleCase(std::string str, char delimiter = '_');
std::string trim(const std::string& str, const std::string& whitespace = " \t");
std::string readFile(const std::string& filepath);
