#pragma once

#include <deque>
#include <vector>

#include "card.h"
#include "city.h"
#include "disease.h"
#include "player.h"

namespace pandemic
{
enum class Difficulty
{
    Introductory = 0,
    Standard,
    Heroic
};

enum class TriggersOutbreak
{
    No,
    Yes
};

class Board
{
public:
    Board() = default;
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
    std::shared_ptr<City>& startCity()
    {
        return m_startingCity;
    }
    std::vector<std::shared_ptr<City>>& cities()
    {
        return m_cities;
    }
    std::vector<std::shared_ptr<City>> researchStationCities() const;

    std::shared_ptr<InfectionCard> infect();
    std::shared_ptr<PlayerCard> drawPlayerCard();
    void addCardToPlayerDiscardPile(const std::shared_ptr<PlayerCard>& card);
    size_t numPlayerCards() const
    {
        return m_playerDeck.size();
    }

    int infectionRate() const;
    int numOutbreaks() const
    {
        return m_numOutbreaks;
    }
    int numDiscoveredCures() const;
    std::vector<Cure> cures() const
    {
        return m_cures;
    }
    bool isCureDiscovered(const DiseaseType type) const;
    bool isCureEradicated(const DiseaseType type) const;
    void discoverCure(const DiseaseType type);
    void increaseInfectionRate();
    void epidemicInfection();
    void intensify();
    bool diseaseCubeCountMaxed();
    void addDisease(const std::shared_ptr<City>& city);

private:
    void addDisease(const std::shared_ptr<City>& city,
                    TriggersOutbreak triggersOutbreak,
                    DiseaseType disease);
    int numDiseaseCubesOnMap(DiseaseType type) const;

    // TODO: Should the cards be unique_ptr?
    std::deque<std::shared_ptr<PlayerCard>> m_playerDeck;
    std::deque<std::shared_ptr<PlayerCard>> m_playerDiscardPile;
    std::deque<std::shared_ptr<InfectionCard>> m_infectionDeck;
    std::deque<std::shared_ptr<InfectionCard>> m_infectionDiscardPile;
    std::vector<std::shared_ptr<City>> m_cities;
    std::vector<std::shared_ptr<City>> m_outbreakCities;
    std::shared_ptr<City> m_startingCity;
    int m_numOutbreaks = 0;
    size_t m_infectionRateIndex = 0;
    std::vector<Cure> m_cures;
};

} // namespace pandemic
