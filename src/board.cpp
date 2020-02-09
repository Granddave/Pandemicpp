#include "board.h"

#include <iostream>
#include <memory>
#include <algorithm>
#include <numeric>
#include <assert.h>
#include <ctime>

#include "utils.h"
#include "logging.h"

namespace Pandemic {

Board::Board()
{
}

void Board::reset()
{
    m_playerDeck.clear();
    m_playerDiscardPile.clear();
    m_infectionDeck.clear();
    m_infectionDiscardPile.clear();
    m_cities.clear();
    m_startingCity.reset();
    m_cures.clear();
    m_numOutbreaks = 0;
    m_infectionRateIndex = 0;
}

void Board::initCures()
{
    m_cures.emplace_back(DiseaseType::Yellow);
    m_cures.emplace_back(DiseaseType::Red);
    m_cures.emplace_back(DiseaseType::Blue);
    m_cures.emplace_back(DiseaseType::Black);
}

void Board::initCities(const std::string& str)
{
    CityParser reader(str);
    m_startingCity = reader.startCity();
    m_cities = reader.cities();
}

void Board::createCityCards()
{
    for (auto& city : m_cities)
    {
        m_playerDeck.push_back(std::make_shared<PlayerCityCard>(city));
        m_infectionDeck.push_back(std::make_shared<InfectionCard>(city));
    }
}

void Board::setStartCity(std::shared_ptr<City>& city)
{
    city->setResearchStation(true);
    m_startingCity = city;
}

void Board::initInfections()
{
    LOG_INFO("--- Initializing infections");
    assert(m_infectionDeck.size() > 3*3);
    std::random_shuffle(m_infectionDeck.begin(), m_infectionDeck.end());
    for (int numCubes = 3; numCubes != 0; numCubes--)
    {
        LOG_DEBUG("{} cubes:", numCubes);
        for (int cityIx = 0; cityIx < 3; cityIx++)
        {
            std::shared_ptr<InfectionCard> card = m_infectionDeck.front();
            m_infectionDeck.pop_front();
            assert(card);
            LOG_DEBUG(card->city->name());
            for (int i = 0; i < numCubes; i++)
            {
                addDisease(card->city);
            }
            m_infectionDiscardPile.push_front(card);
        }
    }
}

void Board::insertEventCards()
{
    for (int i = 0; i < c_numEventCards; ++i)
    {
        const auto type = static_cast<EventCardType>(i);
        m_playerDeck.push_back(std::make_shared<EventCard>(type));
    }
}

void Board::distributePlayerCards(std::vector<std::shared_ptr<Player>>& players)
{
    assert(players.size() >= c_minPlayers);
    assert(players.size() <= c_maxPlayers);

    LOG_INFO("--- Distribute player cards");
    std::random_shuffle(m_playerDeck.begin(), m_playerDeck.end());
    for (const std::shared_ptr<Player>& p : players)
    {
        /* Distribute cards to players.
         *  2 players: 4 each
         *  3 players: 3 each
         *  4 players: 2 each */
        LOG_DEBUG("{} gets ", roleToString(p->role()));
        for (size_t i = 0; i < 6 - players.size(); i++)
        {
            std::shared_ptr<PlayerCard> card = m_playerDeck.front();
            LOG_DEBUG(" - {}", card->name());
            m_playerDeck.pop_front();
            p->addCard(card);
        }
    }
}

void Board::insertEpidemicCards(const int numEpidemicCards)
{
    std::random_shuffle(m_playerDeck.begin(), m_playerDeck.end());

    const int numPiles = numEpidemicCards;
    const int deckSize = static_cast<int>(m_playerDeck.size());
    const int subPileSize = deckSize / numPiles;
    int remainder = deckSize % numPiles;
    auto begin = m_playerDeck.begin();
    auto end = begin;

    for (int subPileIx = 0; subPileIx < numPiles; subPileIx++)
    {
        end += (remainder > 0) ? (subPileSize + !!(remainder--)) : subPileSize;
        end = m_playerDeck.insert(end, std::make_shared<EpidemicCard>());
        end++;
        std::random_shuffle(begin, end);
        begin = end;
    }
}

std::shared_ptr<City> Board::city(const std::string& cityName)
{
    auto city = std::find_if(m_cities.begin(), m_cities.end(),
                                  [&](const std::shared_ptr<City>& c)
    {
        return c->name() == cityName;
    });
    assert(*city);
    return *city;
}

std::vector<std::shared_ptr<City>> Board::researchStationCities() const
{
    std::vector<std::shared_ptr<City>> researchCities;
    for (const auto& city : m_cities)
    {
        if (city->hasResearchStation())
        {
            researchCities.push_back(city);
        }
    }
    return researchCities;
}

std::shared_ptr<InfectionCard> Board::infect()
{
    assert(m_infectionDeck.size() > 0);
    auto card = m_infectionDeck.front();
    m_infectionDeck.pop_front();

    LOG_INFO("Infecting {}", card->city->name());
    addDisease(card->city);

    m_infectionDiscardPile.push_front(card);
    return card;
}

std::shared_ptr<PlayerCard> Board::drawPlayerCard()
{
    if (m_playerDeck.size() > 0)
    {
        auto card = m_playerDeck.front();
        m_playerDeck.pop_front();
        return card;
    }
    return nullptr;
}

int Board::infectionRate() const
{
    if (m_infectionRateIndex < c_infectionRateSize)
    {
        return c_infectionRates[m_infectionRateIndex];
    }
    return c_infectionRates[c_infectionRateSize-1];
}

int Board::numDiscoveredCures() const
{
    int discoveredCures = 0;
    for (const auto& cure : m_cures)
    {
        discoveredCures += cure.discovered ? 1 : 0;
    }
    return discoveredCures;
}

bool Board::isCureDiscovered(const DiseaseType type) const
{
    for (const Cure& cure : m_cures)
    {
        if (cure.type == type)
        {
            return cure.discovered;
        }
    }

    LOG_WARN("Didn't find the cure {} in the list", type);
    return false;
}

bool Board::isCureEradicated(const DiseaseType type) const
{
    if (isCureDiscovered(type) == false)
    {
        return false;
    }

    return numDiseaseCubesOnMap(type) == 0;
}

void Board::discoverCure(const DiseaseType type)
{
    for (Cure& cure : m_cures)
    {
        if (cure.type == type)
        {
            cure.discovered = true;
            return;
        }
    }
}

void Board::increaseInfectionRate()
{
    if (m_infectionRateIndex++ == c_infectionRateSize)
    {
        m_infectionRateIndex = c_infectionRateSize-1;
    }
}

void Board::epidemicInfection()
{
    auto backCard = m_infectionDeck.back();
    m_infectionDeck.pop_back();

    auto city = backCard->city;
    const int numCubesAlready = city->numDiseaseCubes(city->diseaseType());
    const int numCubesToAdd = (numCubesAlready == 0) ? 3 : 4-numCubesAlready;

    // Add 3 cubes, or enough cubes to trigger outbreak
    for (int i = 0; i < numCubesToAdd; ++i)
    {
        addDisease(city, false, city->diseaseType());
    }
    m_outbreakCities.clear();

    m_infectionDiscardPile.push_front(backCard);
}

void Board::intensify()
{
    std::random_shuffle(m_infectionDiscardPile.begin(), m_infectionDiscardPile.end());

    m_infectionDeck.insert(m_infectionDeck.end(),
                           m_infectionDiscardPile.begin(),
                           m_infectionDiscardPile.end());
    m_infectionDiscardPile.clear();
}

bool Board::diseaseCubeCountMaxed()
{
    for (int i = 0; i < c_numDiseases; ++i)
    {
        auto type = static_cast<DiseaseType>(i);
        if (numDiseaseCubesOnMap(type) > c_maxPlacedDiseaseCubes)
        {
            return true;
        }
    }
    return false;
}

void Board::addDisease(std::shared_ptr<City> city)
{
    LOG_TRACE("NEW Disease in {}", city->name());
    addDisease(city, false, city->diseaseType());
    m_outbreakCities.clear();
}

void Board::addDisease(std::shared_ptr<City> city, bool outbreak, DiseaseType disease)
{
    if (std::find(m_outbreakCities.begin(), m_outbreakCities.end(), city)
        != m_outbreakCities.end())
    {
        LOG_DEBUG("{} already had an outbreak", city->name());
        return;
    }

    if (!outbreak)
    {
        disease = city->diseaseType();
    }

    const bool triggeredOutbreak = city->addDisease(disease);
    LOG_INFO("Added {} disease to {} ({})",
             diseaseToString(disease),
             city->name(),
             city->numDiseaseCubes(disease));
    if (triggeredOutbreak)
    {
        m_numOutbreaks++;
        LOG_INFO("Outbreak no.{} in {}!", m_numOutbreaks, city->name());
        m_outbreakCities.push_back(city);
        for (auto& neighbour : city->neighbours())
        {
            addDisease(neighbour, true, disease);
        }
    }
}

int Board::numDiseaseCubesOnMap(DiseaseType type) const
{
    int count = 0;
    for (const auto& city : m_cities)
    {
        count += city->numDiseaseCubes(type);
    }
    return count;
}

}
