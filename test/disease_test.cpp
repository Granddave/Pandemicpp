#include "catch2/catch.hpp"

#include "board.h"
#include "city.h"
#include "commondata_test.h"
#include "disease.h"
#include "utils.h"

namespace pandemic
{

TEST_CASE("Infection Rate")
{
    Board board;
    REQUIRE(board.infectionRate() == c_infectionRates.front());

    // Increase more than allowed
    for (size_t i = 0; i < c_infectionRates.size() + 2; ++i)
    {
        const int rate =
            i < c_infectionRates.size() ? c_infectionRates.at(i) : c_infectionRates.back();
        REQUIRE(board.infectionRate() == rate);

        board.increaseInfectionRate();
    }

    // Infection rate is maxed out
    const int highestRate = c_infectionRates.back();
    REQUIRE(board.infectionRate() == highestRate);
}

TEST_CASE("Adding and curing a disease")
{
    const auto disease = DiseaseType::Blue;
    City city("atlanta", disease);

    // Add more diseases than a city can hold
    for (int i = 1; i <= c_maxCubesInCity + 1; ++i)
    {
        CAPTURE(i);
        const bool triggeredOutbreak = city.addDisease(disease);
        if (i <= c_maxCubesInCity)
        {
            REQUIRE(city.numDiseaseCubes() == i);
            REQUIRE(triggeredOutbreak == false);
        }
        else
        {
            REQUIRE(city.numDiseaseCubes() == c_maxCubesInCity);
            REQUIRE(triggeredOutbreak == true);
        }
    }

    // Cure diseases
    for (int i = c_maxCubesInCity; i != 0; --i)
    {
        CAPTURE(i);
        city.cureDisease(disease);
        REQUIRE(city.numDiseaseCubes() == i - 1);
    }

    // Cure a disease free city
    REQUIRE(city.numDiseaseCubes() == 0);
    city.cureDisease(disease);
    REQUIRE(city.numDiseaseCubes() == 0);
}

TEST_CASE("Cures are not discovered after initialization")
{
    Board board;
    board.initCures();

    REQUIRE(board.numDiscoveredCures() == 0);
    for (const auto& cure : board.cures())
    {
        REQUIRE_FALSE(board.isCureDiscovered(cure.type));
    }
}

TEST_CASE("Cure is discovered")
{
    Board board;
    board.initCures();

    for (int i = 0; i < c_numCures; ++i)
    {
        const auto type = static_cast<DiseaseType>(i);
        REQUIRE_FALSE(board.isCureDiscovered(type));

        board.discoverCure(type);
        REQUIRE(board.isCureDiscovered(type));

        board.discoverCure(type);
        REQUIRE(board.isCureDiscovered(type));
        REQUIRE(board.numDiscoveredCures() == i + 1);
    }
}

TEST_CASE("Maxed out disease cubes on the map")
{
    Board board;
    board.initCures();
    board.initCities(readFile("cities_data.txt"));

    REQUIRE_FALSE(board.diseaseCubeCountMaxed());

    const auto diseaseType = DiseaseType::Blue;
    int count = 0;
    for (auto& city : board.cities())
    {
        if (city->diseaseType() != diseaseType)
        {
            continue;
        }

        for (int i = 0; i < c_maxCubesInCity; ++i)
        {
            CAPTURE(city->name());
            CAPTURE(count);
            CAPTURE(c_maxPlacedDiseaseCubes);

            if (count <= c_maxPlacedDiseaseCubes)
            {
                REQUIRE_FALSE(board.diseaseCubeCountMaxed());
            }
            else
            {
                REQUIRE(board.diseaseCubeCountMaxed());
            }

            board.addDisease(city);
            count++;
        }
    }
    REQUIRE(board.diseaseCubeCountMaxed());
}

} // namespace pandemic
