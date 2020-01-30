#include "catch.hpp"

#include "disease.h"
#include "board.h"
#include "city.h"
#include "commondata_test.h"

namespace Pandemic
{

TEST_CASE("Infection Rate")
{
    Board board;

    REQUIRE(board.getInfectionRate() == c_infectionRates[0]);

    // Increase more than allowed
    for (int i = 0; i < c_infectionRateSize+2; ++i)
    {
        const int rate = i < c_infectionRateSize ?
                    c_infectionRates[i] :
                    c_infectionRates[c_infectionRateSize-1];
        REQUIRE(board.getInfectionRate() == rate);

        board.increaseInfectionRate();
    }

    // Rate is maxed out
    const int highestRate = c_infectionRates[c_infectionRateSize-1];
    REQUIRE(board.getInfectionRate() == highestRate);
}

TEST_CASE("Adding and curing a disease")
{
    auto disease = DiseaseType::Blue;
    City city("atlanta", disease);

    // Add diseases
    for (int i = 1; i <= c_maxCubesInCity+1; ++i)
    {
        CAPTURE(i);
        bool triggeredOutbreak = city.addDisease(disease);
        if (i <= c_maxCubesInCity)
        {
            REQUIRE(getNumDiseaseCubes(city.getDiseaseCubes()) == i);
            REQUIRE(triggeredOutbreak == false);
        }
        else
        {
            REQUIRE(getNumDiseaseCubes(city.getDiseaseCubes()) == c_maxCubesInCity);
            REQUIRE(triggeredOutbreak == true);
        }
    }

    // Cure diseases
    for (int i = c_maxCubesInCity; i != 0; --i)
    {
        CAPTURE(i);
        city.cureDisease(disease);
        REQUIRE(getNumDiseaseCubes(city.getDiseaseCubes()) == i-1);
    }

    // Cure a disease free city
    city.cureDisease(disease);
    REQUIRE(getNumDiseaseCubes(city.getDiseaseCubes()) == 0);
}

}
