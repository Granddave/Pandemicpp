#include "game.h"

#include <iostream>
#include <assert.h>
#include <algorithm>

#include "utils.h"

namespace Pandemic {

Game::Game(const Config& config)
    : m_config(config)
{
    unsigned int seed = m_config.seed;
    if (m_config.seed == 0)
    {
        seed = static_cast<unsigned int>(time(nullptr));
    }
    std::cout << "seed: " << seed << "\n";
    srand(seed);

    init();
    printStatus();
    reset();
}

void Game::init()
{
    m_board.initCures();
    m_board.initCities(readFile(m_config.citiesFile));
    m_board.createCityCards();
    m_board.initInfections();
    m_board.insertEventCards();
    initPlayers(m_config.numPlayers);
    m_board.distributePlayerCards(m_players);
    m_board.insertEpidemicCards(numEpidemicCards(m_config.difficulty));
}

void Game::reset()
{
    m_board.reset();
    m_players.clear();
    m_currentPlayer = 0;
    m_gameOver = false;
}

void Game::printStatus()
{
    std::cout << "\n--- Game status\n"
              << "- Players:\n";
    for (const auto& player : m_players)
    {
        std::cout << roleToString(player->getRole()) << " is in "
                  << player->getCurrentCity()->getName()
                  << " and has " <<'\n';

        for (const auto& card : player->getCards())
        {
            std::cout << " * " << card->getName() << '\n';
        }
    }

    std::cout << "\n- Diseases:\n";
    for (const auto& city : m_board.getCities())
    {
        const int numCubes = getNumDiseaseCubes(city->getDiseaseCubes());
        if (numCubes > 0)
        {
            std::cout << ' ' << city->getName() << " - " << numCubes << '\n';
        }
    }
    std::cout << std::endl;
}

void Game::initPlayers(const int numPlayers)
{
    assert(numPlayers >= c_minPlayers);
    assert(numPlayers <= c_maxPlayers);

    // Find start city
    auto cities = m_board.getCities();
    auto startCity = m_board.getStartCity();
    assert(startCity);

    for (int playerIx = 0; playerIx < numPlayers; playerIx++)
    {
        std::shared_ptr<Player> player = std::make_shared<Player>();
        player->setCurrentCity(startCity);

        // Randomize the players role
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

int Game::numEpidemicCards(Difficulty difficulty) const
{
    switch (difficulty)
    {
        case Difficulty::Easy:   return 4;
        case Difficulty::Medium: return 5;
        case Difficulty::Hard:   return 6;
    }
    return 4;
}

}
