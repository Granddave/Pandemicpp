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

    REQUIRE(city.getName() == "London");
    REQUIRE(city.getDiseaseType() == DiseaseType::Blue);
    REQUIRE(city.getHasResearchStation() == false);
    REQUIRE(city.getNumDiseaseCubes() == 0);
}

TEST_CASE("Parse cities from string")
{
    CityParser parser(c_exampleCities);

    auto cities = parser.getCities();
    auto startCity = parser.getStartCity();
    REQUIRE(cities.size() == 4);
    REQUIRE(startCity->getName() == "B");
    REQUIRE(startCity->getDiseaseType() == DiseaseType::Yellow);
    REQUIRE(cities.at(3)->getDiseaseType() == DiseaseType::Red);
    REQUIRE(cities.at(0)->getNeighbours().at(0)->getName() == cities.at(1)->getName());
}

TEST_CASE("Outbreaks")
{
    Board board;
    board.initCures();
    board.initCities(readFile("cities_data.txt"));

    for (int i = 0; i < c_maxCubesInCity; ++i) {
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

}
