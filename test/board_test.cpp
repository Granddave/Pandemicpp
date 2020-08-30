#include "catch2/catch.hpp"

#include "board.h"
#include "commondata_test.h"

namespace Pandemic
{
// Class to expose for testing
class TestBoard : Board
{
public:
    TestBoard();
    std::deque<std::shared_ptr<PlayerCard>>& playerDeck()
    {
        return m_playerDeck;
    }
    // std::deque<std::shared_ptr<PlayerCard>>& playerDiscardPile;
    // std::deque<std::shared_ptr<InfectionCard>>& infectionDeck;
    // std::deque<std::shared_ptr<InfectionCard>>& infectionDiscardPile;
};

TEST_CASE("Cure discovering")
{
    Board board;

    board.initCures();
    REQUIRE(board.numDiscoveredCures() == 0);

    SECTION("Discover a cure")
    {
        board.discoverCure(DiseaseType::Red);
        REQUIRE(board.isCureDiscovered(DiseaseType::Red));
        REQUIRE(board.numDiscoveredCures() == 1);
    }

    SECTION("Discover two cures")
    {
        board.discoverCure(DiseaseType::Red);
        board.discoverCure(DiseaseType::Blue);
        REQUIRE(board.isCureDiscovered(DiseaseType::Red));
        REQUIRE(board.isCureDiscovered(DiseaseType::Blue));
        REQUIRE(board.numDiscoveredCures() == 2);
    }

    SECTION("Discover all cures")
    {
        for (int i = 0; i < c_numCures; ++i)
        {
            DiseaseType type = static_cast<DiseaseType>(i);
            board.discoverCure(type);
            REQUIRE(board.isCureDiscovered(type));
            REQUIRE(board.numDiscoveredCures() == i + 1);
        }
    }
}

TEST_CASE("Eradication")
{
    Board board;
    board.initCures();
    board.initCities(c_exampleCities);
    auto disease = DiseaseType::Yellow;

    REQUIRE_FALSE(board.isCureEradicated(disease));

    SECTION("One city, one disease")
    {
        board.city("A")->addDisease(disease);

        REQUIRE_FALSE(board.isCureEradicated(disease));

        board.discoverCure(disease);
        board.city("A")->cureDisease(disease);
    }

    REQUIRE(board.isCureEradicated(disease));
}

TEST_CASE("Maxed out on disease cubes")
{
    // Todo
}

TEST_CASE("Cities with research stations")
{
    // Todo
}

TEST_CASE("Init cities")
{
    // Todo
}

TEST_CASE("Init cards")
{
    // Todo
}

TEST_CASE("Init infections")
{
    // Todo
}

TEST_CASE("Init player cards")
{
    // Todo
}

TEST_CASE("Infect a city with Infection card")
{
    // Todo
}

} // namespace Pandemic
