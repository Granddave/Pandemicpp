#pragma once

#include <map>
#include <numeric>
#include <string>
#include <array>

namespace pandemic {

const int c_numDiseases = 4;
const int c_numCures = c_numDiseases;
const int c_numCardsToCure = 5;
const int c_maxPlacedDiseaseCubes = 24; // Total nof cubes of a disease in the game
const int c_maxCubesInCity = 3;
const int c_maxOutbreaks = 8;
constexpr std::array<int, 7> c_infectionRates = {2, 2, 2, 3, 3, 4, 4};

enum class DiseaseType
{
    Yellow = 0,
    Red = 1,
    Blue = 2,
    Black = 3
};

using DiseaseCube = DiseaseType;

inline std::string diseaseToString(const DiseaseType disease)
{
    switch (disease)
    {
        case DiseaseType::Yellow: return "Yellow";
        case DiseaseType::Red:    return "Red";
        case DiseaseType::Blue:   return "Blue";
        case DiseaseType::Black:  return "Black";
    }
    return "Unknown disease";
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
