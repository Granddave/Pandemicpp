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
    Introductory = 0,
    Standard,
    Heroic
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

    std::shared_ptr<City> city(const std::string& cityName);
    std::shared_ptr<City>& startCity() { return m_startingCity; }
    std::vector<std::shared_ptr<City>>& cities() { return m_cities; }
    std::vector<std::shared_ptr<City>> researchStationCities() const;

    std::shared_ptr<InfectionCard> infect();
    std::shared_ptr<PlayerCard> drawPlayerCard();
    size_t numPlayerCards() const { return m_playerDeck.size(); }

    int infectionRate() const;
    int numOutbreaks() const { return m_numOutbreaks; }
    int numDiscoveredCures() const;
    bool isCureDiscovered(const DiseaseType type) const;
    bool isCureEradicated(const DiseaseType type) const;
    void discoverCure(const DiseaseType type);
    void increaseInfectionRate();
    void epidemicInfection();
    void intensify();
    bool diseaseCubeCountMaxed();
    void addDisease(std::shared_ptr<City> city);

private:
    void addDisease(std::shared_ptr<City> city, bool outbreak, DiseaseType disease);
    int numDiseaseCubesOnMap(DiseaseType type) const;

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
