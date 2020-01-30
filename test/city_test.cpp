#include "catch.hpp"

#include "board.h"
#include "city.h"
#include "disease.h"
#include "utils.h"

#include "commondata_test.h"

TEST_CASE("City creation")
{
    Pandemic::City city("atlanta", Pandemic::DiseaseType::Blue);

    REQUIRE(city.getName() == "atlanta");
    REQUIRE(city.getDiseaseType() == Pandemic::DiseaseType::Blue);
    REQUIRE(city.getHasResearchStation() == false);
    REQUIRE(getNumDiseaseCubes(city.getDiseaseCubes()) == 0);
}

TEST_CASE("Parse cities from string")
{
    Pandemic::CityParser parses(c_exampleCities);

    auto cities = parses.getCities();
    auto startCity = parses.getStartCity();
    REQUIRE(cities.size() == 4);
    REQUIRE(startCity->getName() == "B");
    REQUIRE(startCity->getDiseaseType() == Pandemic::DiseaseType::Yellow);
    REQUIRE(cities.at(3)->getDiseaseType() == Pandemic::DiseaseType::Red);
    REQUIRE(cities.at(0)->getNeighbours().at(0)->getName() == cities.at(1)->getName());
}

TEST_CASE("Outbreaks")
{
    Pandemic::Board board;
    board.initCures();
    board.initCities(readFile("cities_data.txt"));

    for (int i = 0; i < Pandemic::c_maxCubesInCity; ++i) {
        board.addDisease(board.getCity("Madrid"));
        board.addDisease(board.getCity("Algiers"));
        board.addDisease(board.getCity("Cairo"));
    }

    board.addDisease(board.getCity("Paris"));
    board.addDisease(board.getCity("Paris"));
    board.addDisease(board.getCity("Istanbul"));

    SECTION("Trigger outbreak")
    {
        board.addDisease(board.getCity("Madrid"));
        REQUIRE(board.getNumOutbreaks() == 1);
    }

    SECTION("Trigger chain outbreaks")
    {
        board.addDisease(board.getCity("Algiers"));
        REQUIRE(board.getNumOutbreaks() == 2);
    }
}
