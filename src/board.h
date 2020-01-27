#pragma once

#include <vector>
#include <deque>

#include "city.h"
#include "disease.h"
#include "player.h"
#include "card.h"

namespace Pandemic {

enum class Difficulty
{
    Easy,
    Medium,
    Hard
};

struct Config
{
    int numPlayers = 2;
    Difficulty difficulty = Difficulty::Easy;
    std::string citiesFile = "cities_data.txt";
    unsigned int seed = 0;
};

class Board
{
public:
    Board(const Config& config);
    void reset();
    void init();
    void printStatus();

private:
    void initCures();
    void initCities();
    void createCityCard(const std::shared_ptr<City> &city);
    void setStartCity(std::shared_ptr<City>& city);
    void initInfections();
    void initPlayers(const int numPlayers);
    void insertEventCards();
    void distributePlayerCards();
    void insertEpidemicCards(const int numEpidemicCards);
    std::shared_ptr<City> getCity(const std::string& cityName);

    Config m_config;
    std::deque<std::shared_ptr<PlayerCard>> m_playerDeck;
    std::deque<std::shared_ptr<PlayerCard>> m_playerDiscardPile;
    std::deque<std::shared_ptr<InfectionCard>> m_infectionDeck;
    std::deque<std::shared_ptr<InfectionCard>> m_infectionDiscardPile;
    std::vector<std::shared_ptr<City>> m_cities;
    std::shared_ptr<City> m_mainCity;
    int m_numOutbreaks = 0;
    int m_infectionRateIndex = 0;
    std::vector<Cure> m_cures;
    std::vector<std::shared_ptr<Player>> m_players;
};

}
