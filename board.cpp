#include "board.h"

#include <iostream>
#include <memory>
#include <algorithm>
#include <assert.h>
#include <ctime>

namespace Pandemic {

Board::Board()
{
#if 0
    auto seed = static_cast<unsigned int>(time(nullptr));
#else
    unsigned int seed = 1;
#endif
    std::cout << "seed: " << seed << "\n";
    srand(seed);

    init();
}

void Board::reset()
{
    m_playerDeck.clear();
    m_playerDiscardPile.clear();
    m_infectionDeck.clear();
    m_infectionDiscardPile.clear();
    m_players.clear();
    m_cities.clear();
    m_mainCity.reset();
    m_cures.clear();
    m_numOutbreaks = 0;
    m_infectionRateIndex = 0;
}

void Board::init()
{
    initCures();
    initCities();
    initInfections();
    insertEventCards();
    initPlayers(2);
    distributePlayerCards();
    insertEpidemicCards(4);
}

void Board::initCures()
{
    m_cures.emplace_back(DiseaseType::Yellow);
    m_cures.emplace_back(DiseaseType::Red);
    m_cures.emplace_back(DiseaseType::Blue);
    m_cures.emplace_back(DiseaseType::Black);
}

void Board::initCities()
{
    CityReader reader("/home/david/Dev/pandemic/cities_data.txt");
    m_mainCity = reader.getMainCity();
    m_cities = reader.getCities();

    for (const auto& city : m_cities)
    {
        createCityCard(city);
    }
}

void Board::createCityCard(std::shared_ptr<City> city)
{
    m_playerDeck.push_back(std::make_shared<PlayerCityCard>(city));
    m_infectionDeck.push_back(std::make_shared<InfectionCard>(city));
}

void Board::setStartCity(std::shared_ptr<City> city)
{
    city->setHasResearchStation(true);
    m_mainCity = city;
}

void Board::initInfections()
{
    std::cout << "--- Initializing infections\n";
    assert(m_infectionDeck.size() > 3*3);
    std::random_shuffle(m_infectionDeck.begin(), m_infectionDeck.end());
    for (int numCubes = 3; numCubes != 0; numCubes--)
    {
        std::cout << numCubes << " cubes:\n";
        for (int cityIx = 0; cityIx < 3; cityIx++)
        {
            std::shared_ptr<InfectionCard> card = m_infectionDeck.front();
            m_infectionDeck.pop_front();
            assert(card);
            std::cout << " " << card->city->getName() << "\n";
            for (int i = 0; i < numCubes; i++)
            {
                card->city->addDisease();
            }
            m_infectionDiscardPile.push_front(card);
        }
    }
}

void Board::initPlayers(const int numPlayers)
{
    assert(numPlayers >= 2);
    assert(numPlayers <= 4);

    // Find start city
    auto startCity = std::find_if(m_cities.begin(), m_cities.end(),
                                  [&](const std::shared_ptr<City>& c)
    {
        return c == m_mainCity;
    });
    assert(*startCity);

    for (int playerIx = 0; playerIx < numPlayers; playerIx++)
    {
        std::shared_ptr<Player> player = std::make_shared<Player>();
        player->setCurrentCity(*startCity);

        while (true)
        {
            Role role = static_cast<Role>(std::rand() % c_numRoles);

            auto roleCollision = std::find_if(m_players.begin(), m_players.end(),
                                              [&](const std::shared_ptr<Player>& p)
            {
                return p->getRole() == role;
            });

            if (roleCollision != m_players.end())
            {
                continue;
            }

            player->setRole(role);
            break;
        }

        m_players.push_back(player);
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

void Board::distributePlayerCards()
{
    assert(m_players.size() <= 4);

    std::cout << "\n--- Distribute player cards\n";
    std::random_shuffle(m_playerDeck.begin(), m_playerDeck.end());
    for (const std::shared_ptr<Player>& p : m_players)
    {
        /* Distribute cards to players.
         *  2 players: 4 each
         *  3 players: 3 each
         *  4 players: 2 each */
        std::cout << roleToString(p->getRole()) << " gets \n";
        for (size_t i = 0; i < 6 - m_players.size(); i++)
        {
            std::shared_ptr<PlayerCard> card = m_playerDeck.front();
            std::cout << " - " << card->getName() << "\n";
            m_playerDeck.pop_front();
            p->addCard(card);
        }
    }
}

void Board::insertEpidemicCards(const int numEpidemicCards)
{
    std::random_shuffle(m_playerDeck.begin(), m_playerDeck.end());

    const int piles = numEpidemicCards;
    const int deckSize = static_cast<int>(m_playerDeck.size());
    const int subPileSize = deckSize / piles;
    int remainder = deckSize % piles;
    auto begin = m_playerDeck.begin();
    auto end = begin;

    for (int subPileIx = 0; subPileIx < piles; subPileIx++)
    {
        end += (remainder > 0) ? (subPileSize + !!(remainder--)) : subPileSize;
        end = m_playerDeck.insert(end, std::make_shared<EpidemicCard>());
        end++;
        std::random_shuffle(begin, end);
        begin = end;
    }
}

}
