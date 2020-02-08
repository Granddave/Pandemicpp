#include "catch2/catch.hpp"

#include "disease.h"
#include "board.h"
#include "city.h"
#include "commondata_test.h"

namespace Pandemic
{

TEST_CASE("Infection Rate")
{
    Board board;
    REQUIRE(board.infectionRate() == c_infectionRates[0]);

    // Increase more than allowed
    for (int i = 0; i < c_infectionRateSize+2; ++i)
    {
        const int rate = i < c_infectionRateSize ?
                    c_infectionRates[i] :
                    c_infectionRates[c_infectionRateSize-1];
        REQUIRE(board.infectionRate() == rate);

        board.increaseInfectionRate();
    }

    // Infection rate is maxed out
    const int highestRate = c_infectionRates[c_infectionRateSize-1];
    REQUIRE(board.infectionRate() == highestRate);
}

TEST_CASE("Adding and curing a disease")
{
    const auto disease = DiseaseType::Blue;
    City city("atlanta", disease);

    // Add more diseases than a city can hold
    for (int i = 1; i <= c_maxCubesInCity+1; ++i)
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
        REQUIRE(city.numDiseaseCubes() == i-1);
    }

    // Cure a disease free city
    REQUIRE(city.numDiseaseCubes() == 0);
    city.cureDisease(disease);
    REQUIRE(city.numDiseaseCubes() == 0);
}

}
