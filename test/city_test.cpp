#include "catch.hpp"

#include "board.h"
#include "city.h"
#include "disease.h"

TEST_CASE("Test")
{
    Pandemic::City city("atlanta", Pandemic::DiseaseType::Red);

    REQUIRE(city.getName() == "atlanta");
    REQUIRE(city.getDiseaseType() == Pandemic::DiseaseType::Red);
}

TEST_CASE("Parse cities from string")
{
    Pandemic::CityReader reader;
    reader.parseString(
R"(0 A B D
*0 B A D
0 C A
1 D A B)");

   /* City network described above:
    *
    * A---B
    * | \ |
    * |  \|
    * C   D
    *
    */

    auto cities = reader.getCities();
    auto startCity = reader.getStartCity();
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
    board.initCities("cities_data.txt");
    std::shared_ptr<Pandemic::City> madrid = board.getCity("Madrid");

    for (int i = 0; i < 3; ++i) {
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
