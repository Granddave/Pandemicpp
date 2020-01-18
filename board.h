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

private:
    void initCures();
    void initCities();
    void initInfections();
    void initPlayers(const int numPlayers);
    void initPlayerCards();
    void addCity(std::shared_ptr<City> city);

    std::deque<std::shared_ptr<PlayerCard>> m_playerDeck;
    std::deque<std::shared_ptr<PlayerCard>> m_playerDiscardPile;
    std::deque<std::shared_ptr<InfectionCard>> m_infectionDeck;
    std::deque<std::shared_ptr<InfectionCard>> m_infectionDiscardPile;
    std::vector<std::shared_ptr<City>> m_cities;
    int m_numOutbreaks = 0;
    int m_infectionRateIndex = 0;
    std::vector<Cure> m_cures;
    std::vector<std::shared_ptr<Player>> m_players;
};

}
