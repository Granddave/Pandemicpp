#include "catch2/catch.hpp"

#include "board.h"
#include "city.h"
#include "disease.h"
#include "utils.h"

#include "commondata_test.h"

namespace Pandemic
{

TEST_CASE("City creation")
{
    City city("london", DiseaseType::Blue);

    REQUIRE(city.name() == "London");
    REQUIRE(city.diseaseType() == DiseaseType::Blue);
    REQUIRE(city.hasResearchStation() == false);
    REQUIRE(city.numDiseaseCubes() == 0);
}

TEST_CASE("Parse cities from string")
{
    CityParser parser(c_exampleCities);

    auto cities = parser.cities();
    auto startCity = parser.startCity();
    REQUIRE(cities.size() == 4);
    REQUIRE(startCity->name() == "B");
    REQUIRE(startCity->diseaseType() == DiseaseType::Yellow);
    REQUIRE(cities.at(3)->diseaseType() == DiseaseType::Red);
    REQUIRE(cities.at(0)->neighbours().at(0)->name() == cities.at(1)->name());
}

TEST_CASE("Outbreaks")
{
    Board board;
    board.initCures();
    board.initCities(readFile("cities_data.txt"));

    for (int i = 0; i < c_maxCubesInCity; ++i) {
        board.addDisease(board.city("Madrid"));
        board.addDisease(board.city("Algiers"));
        board.addDisease(board.city("Cairo"));
    }

    board.addDisease(board.city("Paris"));
    board.addDisease(board.city("Paris"));
    board.addDisease(board.city("Istanbul"));

    SECTION("Trigger outbreak")
    {
        board.addDisease(board.city("Madrid"));
        REQUIRE(board.numOutbreaks() == 1);
    }

    SECTION("Trigger chain outbreaks")
    {
        board.addDisease(board.city("Algiers"));
        REQUIRE(board.numOutbreaks() == 2);
    }
}

}
