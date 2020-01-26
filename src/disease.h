#pragma once

#include <map>
#include <numeric>

namespace Pandemic {

const int c_numDiseases = 4;
const int c_maxDiseaseCubes = 24; // Total nof cubes of a disease in the game
const int c_maxCubesInCity = 3;
const int c_maxOutbreaks = 8;
const int c_infectionRates[] = { 2, 2, 2, 3, 3, 4, 4 };

enum class DiseaseType
{
    Yellow,
    Red,
    Blue,
    Black
};

using DiseaseCube = DiseaseType;

inline int getNumDiseaseCubes(const std::map<DiseaseCube, int>& cubes)
{
    return std::accumulate(
                std::begin(cubes), std::end(cubes), 0,
                [] (int value, const std::map<Pandemic::DiseaseType, int>::value_type& p)
    {
        return value + p.second;
    });
}

struct Cure
{
    Cure(DiseaseType type)
        : type(type)
    {
    }

    DiseaseType type;
    bool discovered = false;
    bool eradicated = false;
};

}
