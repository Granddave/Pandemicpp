#include "catch.hpp"

#include "board.h"
#include "city.h"
#include "disease.h"
#include "utils.h"

const std::string c_exampleCities = R"(0 A B D
                                       *0 B A D
                                       0 C A
                                       1 D A B)";
/* City network described above:
 * A---B
 * | \ |
 * |  \|
 * C   D
 *
 * A, B, C disease type 0,
 * D disease type 1
 * B is the start city
 */

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

TEST_CASE("Adding and curing a disease")
{
    auto disease = Pandemic::DiseaseType::Blue;
    Pandemic::City city("atlanta", disease);

    // Add diseases
    for (int i = 1; i <= Pandemic::c_maxCubesInCity+1; ++i)
    {
        CAPTURE(i);
        bool triggeredOutbreak = city.addDisease(disease);
        if (i <= Pandemic::c_maxCubesInCity)
        {
            REQUIRE(getNumDiseaseCubes(city.getDiseaseCubes()) == i);
            REQUIRE(triggeredOutbreak == false);
        }
        else
        {
            REQUIRE(getNumDiseaseCubes(city.getDiseaseCubes()) == Pandemic::c_maxCubesInCity);
            REQUIRE(triggeredOutbreak == true);
        }
    }

    // Cure diseases
    for (int i = Pandemic::c_maxCubesInCity; i != 0; --i)
    {
        CAPTURE(i);
        city.cureDisease(disease);
        REQUIRE(getNumDiseaseCubes(city.getDiseaseCubes()) == i-1);
    }

    // Cure a disease free city
    city.cureDisease(disease);
    REQUIRE(getNumDiseaseCubes(city.getDiseaseCubes()) == 0);
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
