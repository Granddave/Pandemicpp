#pragma once

#include <vector>
#include <deque>

#include "city.h"
#include "disease.h"
#include "player.h"
#include "card.h"

namespace Pandemic {

class Board
{
public:
    Board();
    void reset();
    void init();
    void printStatus();

private:
    void initCures();
    void initCities();
    void createCityCard(std::shared_ptr<City> city);
    void setStartCity(std::shared_ptr<City> city);
    void initInfections();
    void initPlayers(const int numPlayers);
    void insertEventCards();
    void distributePlayerCards();
    void insertEpidemicCards(const int numEpidemicCards);

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
