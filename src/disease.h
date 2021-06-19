#pragma once

#include <array>
#include <map>
#include <numeric>
#include <string>

#include "utils.h"

namespace pandemic
{
constexpr int c_numDiseases = 4;
constexpr int c_numCures = c_numDiseases;
constexpr int c_numCardsToCure = 5;
constexpr int c_maxPlacedDiseaseCubes = 24; // Total nof cubes of a disease in the game
constexpr int c_maxCubesInCity = 3;
constexpr int c_maxOutbreaks = 8;
constexpr std::array<int, 7> c_infectionRates = {2, 2, 2, 3, 3, 4, 4};

enum class DiseaseType
{
    Yellow = 0,
    Red = 1,
    Blue = 2,
    Black = 3
};

inline std::string diseaseToString(const DiseaseType disease)
{
    switch (disease)
    {
        case DiseaseType::Yellow:
            return "Yellow";
        case DiseaseType::Red:
            return "Red";
        case DiseaseType::Blue:
            return "Blue";
        case DiseaseType::Black:
            return "Black";
    }

    VERIFY_NOT_REACHED();
    return "Unknown disease";
}

struct Cure
{
    explicit Cure(DiseaseType t) : type(t)
    {
    }

    DiseaseType type;
    bool discovered = false;
    bool eradicated = false;
};

} // namespace pandemic
