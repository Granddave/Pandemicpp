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

class Board
{
public:
    Board();
    void reset();

    void initCures();
    void initCities(const std::string& str);
    void createCityCards();
    void setStartCity(std::shared_ptr<City>& city);
    void initInfections();
    void insertEventCards();
    void distributePlayerCards(std::vector<std::shared_ptr<Player>>& players);
    void insertEpidemicCards(const int numEpidemicCards);

    std::shared_ptr<City> getCity(const std::string& cityName);
    std::shared_ptr<City>& getStartCity() { return m_startingCity; }
    std::vector<std::shared_ptr<City>>& getCities() { return m_cities; }

    std::shared_ptr<InfectionCard> infect();

    int getInfectionRate() const;
    void increaseInfectionRate();

    void addDisease(std::shared_ptr<City> city);

    int getNumOutbreaks() const { return m_numOutbreaks; }

private:
    void addDisease(std::shared_ptr<City> city, bool outbreak, DiseaseType disease);

    std::deque<std::shared_ptr<PlayerCard>> m_playerDeck;
    std::deque<std::shared_ptr<PlayerCard>> m_playerDiscardPile;
    std::deque<std::shared_ptr<InfectionCard>> m_infectionDeck;
    std::deque<std::shared_ptr<InfectionCard>> m_infectionDiscardPile;
    std::vector<std::shared_ptr<City>> m_cities;
    std::vector<std::shared_ptr<City>> m_outbreakCities;
    std::shared_ptr<City> m_startingCity;
    int m_numOutbreaks = 0;
    int m_infectionRateIndex = 0;
    std::vector<Cure> m_cures;
};

}
