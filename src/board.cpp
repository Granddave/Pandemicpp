#include "board.h"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>

#include "logging.h"
#include "utils.h"

namespace pandemic
{
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
    VERIFY(m_infectionDeck.size() > 3 * 3);
    shuffle(m_infectionDeck.begin(), m_infectionDeck.end());
    for (int numCubes = 3; numCubes != 0; numCubes--)
    {
        LOG_DEBUG("{} cubes:", numCubes);
        for (int cityIx = 0; cityIx < 3; cityIx++)
        {
            std::shared_ptr<InfectionCard> card = m_infectionDeck.front();
            m_infectionDeck.pop_front();
            VERIFY_LOG(card, "Failed to get card for city {}", cityIx);
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
    VERIFY(players.size() >= c_minPlayers);
    VERIFY(players.size() <= c_maxPlayers);

    const size_t numPlayerCardsToDistribute = 6 - players.size();
    LOG_INFO("--- Distribute player cards");
    shuffle(m_playerDeck.begin(), m_playerDeck.end());
    for (const std::shared_ptr<Player>& p : players)
    {
        /* Distribute cards to players.
         *  2 players: 4 each
         *  3 players: 3 each
         *  4 players: 2 each */
        LOG_DEBUG("{} gets ", roleToString(p->role()));
        for (size_t i = 0; i < numPlayerCardsToDistribute; i++)
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
    shuffle(m_playerDeck.begin(), m_playerDeck.end());

    const auto numPiles = static_cast<size_t>(numEpidemicCards);
    const size_t deckSize = m_playerDeck.size();
    const auto subPileSize = static_cast<long int>(deckSize / numPiles);
    auto remainder = deckSize % numPiles;
    auto begin = m_playerDeck.begin();
    auto end = begin;

    for (size_t subPileIx = 0; subPileIx < numPiles; subPileIx++)
    {
        end += subPileSize;
        if (remainder > 0)
        {
            remainder--;
            end++;
        }
        end = m_playerDeck.insert(end, std::make_shared<EpidemicCard>());
        end++;
        shuffle(begin, end);
        begin = end;
    }
}

std::shared_ptr<City> Board::city(const std::string& cityName)
{
    auto city = std::find_if(m_cities.begin(), m_cities.end(), [&](const std::shared_ptr<City>& c) {
        return c->name() == cityName;
    });
    VERIFY_LOG(*city, "Failed to find city with name '{}'", cityName);
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
    VERIFY(m_infectionDeck.size() > 0);
    auto card = m_infectionDeck.front();
    m_infectionDeck.pop_front();

    LOG_INFO("Infecting {}", card->city->name());
    addDisease(card->city);

    m_infectionDiscardPile.push_front(card);
    return card;
}

std::shared_ptr<PlayerCard> Board::drawPlayerCard()
{
    if (!m_playerDeck.empty())
    {
        auto card = m_playerDeck.front();
        m_playerDeck.pop_front();
        return card;
    }
    return nullptr;
}

void Board::addCardToPlayerDiscardPile(const std::shared_ptr<PlayerCard>& card)
{
    // TODO: Write test
    m_playerDiscardPile.push_front(card);
}

int Board::infectionRate() const
{
    if (m_infectionRateIndex < c_infectionRates.size())
    {
        return c_infectionRates.at(m_infectionRateIndex);
    }
    return c_infectionRates.back();
}

int Board::numDiscoveredCures() const
{
    return static_cast<int>(std::count_if(
        m_cures.cbegin(), m_cures.cend(), [](const Cure cure) { return cure.discovered; }));
}

bool Board::isCureDiscovered(const DiseaseType type) const
{
    auto cure =
        std::find_if(m_cures.begin(), m_cures.end(), [=](const Cure c) { return c.type == type; });

    if (cure != m_cures.end())
    {
        return cure->discovered;
    }

    LOG_ERROR("Didn't find the cure {} in the list", type);
    VERIFY_NOT_REACHED();
}

bool Board::isCureEradicated(const DiseaseType type) const
{
    // TODO: Write test
    if (!isCureDiscovered(type))
    {
        return false;
    }

    return numDiseaseCubesOnMap(type) == 0;
}

void Board::discoverCure(const DiseaseType type)
{
    auto cure =
        std::find_if(m_cures.begin(), m_cures.end(), [=](const Cure c) { return c.type == type; });

    if (cure != m_cures.end())
    {
        cure->discovered = true;
        return;
    }

    LOG_ERROR("Didn't find the cure {} in the list", type);
    VERIFY_NOT_REACHED();
}

void Board::increaseInfectionRate()
{
    m_infectionRateIndex++;
    if (m_infectionRateIndex > c_infectionRates.size() - 1)
    {
        m_infectionRateIndex = c_infectionRates.size() - 1;
    }
}

void Board::epidemicInfection()
{
    // TODO: Write test
    auto topCard = m_infectionDeck.back();
    m_infectionDeck.pop_back();

    auto city = topCard->city;
    const int numCubesAlready = city->numDiseaseCubes(city->diseaseType());
    // const int numCubesToAdd = (numCubesAlready == 0) ? 3 : 4 - numCubesAlready;
    const int numCubesToAdd = 3 - numCubesAlready;

    // Add 3 cubes, or enough cubes to trigger outbreak
    for (int i = 0; i < numCubesToAdd; ++i)
    {
        addDisease(city, TriggersOutbreak::No, city->diseaseType());
    }
    m_outbreakCities.clear();

    m_infectionDiscardPile.push_front(topCard);
}

void Board::intensify()
{
    shuffle(m_infectionDiscardPile.begin(), m_infectionDiscardPile.end());
    m_infectionDeck.insert(
        m_infectionDeck.end(), m_infectionDiscardPile.begin(), m_infectionDiscardPile.end());
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

void Board::addDisease(const std::shared_ptr<City>& city)
{
    LOG_TRACE("NEW Disease in {}", city->name());
    addDisease(city, TriggersOutbreak::No, city->diseaseType());
    m_outbreakCities.clear();
}

void Board::addDisease(const std::shared_ptr<City>& city,
                       TriggersOutbreak triggersOutbreak,
                       DiseaseType disease)
{
    if (std::find(m_outbreakCities.begin(), m_outbreakCities.end(), city) != m_outbreakCities.end())
    {
        LOG_DEBUG("{} already had an outbreak", city->name());
        return;
    }

    if (triggersOutbreak == TriggersOutbreak::No)
    {
        disease = city->diseaseType();
    }

    const bool triggeredOutbreak = city->addDisease(disease);
    LOG_DEBUG("Added {} disease to {} ({})",
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
            addDisease(neighbour, TriggersOutbreak::Yes, disease);
        }
    }
}

int Board::numDiseaseCubesOnMap(DiseaseType type) const
{
    auto count = std::accumulate(m_cities.begin(), m_cities.end(), 0, [&](auto cubes, auto city) {
        return cubes + city->numDiseaseCubes(type);
    });

    return count;
}

} // namespace pandemic
