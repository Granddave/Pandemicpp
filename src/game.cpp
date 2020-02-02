#include "game.h"

#include <iostream>
#include <assert.h>
#include <algorithm>
#include <memory>

#include "utils.h"
#include "logging.h"

namespace Pandemic {

Game::Game(const Config& config)
    : m_config(config)
{
    setupLog(loglevelFromString("info"));
    unsigned int seed = m_config.seed;
    if (m_config.seed == 0)
    {
        seed = static_cast<unsigned int>(time(nullptr));
    }
    LOG_INFO("Seed: {}", seed);
    srand(seed);

    init();
    run();
    reset();
}

void Game::init()
{
    LOG_INFO("=== Initialization started");
    m_board.initCures();
    m_board.initCities(readFile(m_config.citiesFile));
    m_board.createCityCards();
    m_board.initInfections();
    m_board.insertEventCards();
    initPlayers(m_config.numPlayers);
    m_board.distributePlayerCards(m_players);
    m_board.insertEpidemicCards(numEpidemicCards(m_config.difficulty));
    LOG_INFO("=== Initialization complete");
}

void Game::reset()
{
    m_gameOver = false;
    m_gameWon = false;
    m_board.reset();
    m_players.clear();
    m_currentPlayer = 0;
}

void Game::run()
{
    while (continueGame())
    {
        LOG_INFO("--- Player {}'s turn - ", m_currentPlayer+1);
        if (m_currentPlayer == 0)
        {
            m_round++;
            LOG_INFO("Round nr {}", m_round);
        }

        doTurn();
        m_currentPlayer = (m_currentPlayer+1) % m_config.numPlayers;
    }
}

void Game::doTurn()
{
    auto currentPlayer = m_players.at(static_cast<size_t>(m_currentPlayer));
    for (int i = 0; i < c_numActionsPerTurn; ++i)
    {
        // Todo: Let player choose action
    }

    // Draw two player cards
    for (int i = 0; i < c_numPlayerCardsToDraw; ++i)
    {
        auto card = m_board.drawPlayerCard();
        if (!card)
        {
            // No cards left, game over
            return;
        }

        LOG_INFO("Picked up {}", card->getName());

        auto& playersCards = currentPlayer->getCards();
        if (std::dynamic_pointer_cast<EpidemicCard>(card))
        {
            m_board.increaseInfectionRate();
            m_board.epidemicInfection();
            m_board.intensify();
        }
        else
        {
            playersCards.push_back(card);
        }

        while (playersCards.size() > c_handLimit)
        {
            // Todo: Let player choose card to discard

            playersCards.pop_back(); // Will do for now...
        }
    }

    // Infect cities
    for (int i = 0; i < m_board.getInfectionRate(); ++i)
    {
        m_board.infect();
    }
}

void Game::printStatus()
{
    LOG_INFO("--- Game status");
    LOG_INFO("Players:");
    for (const auto& player : m_players)
    {
        LOG_INFO("{} is in {} and has {}",
                 roleToString(player->getRole()),
                 player->getCurrentCity()->getName());
        for (const auto& card : player->getCards())
        {
            LOG_INFO(" * {}", card->getName());
        }
    }

    LOG_INFO("- Diseases:");
    for (const auto& city : m_board.getCities())
    {
        const int numCubes = getNumDiseaseCubes(city->getDiseaseCubes());
        if (numCubes > 0)
        {
            LOG_INFO("{} - {}", city->getName(), numCubes);
        }
    }
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

bool Game::continueGame()
{
    if (gameOver())
    {
        m_gameOver = true;
        LOG_INFO("Game Over!");
        return false;
    }
    else if (m_board.getNumDiscoveredCures() == c_numDiseases)
    {
        m_gameWon = true;
        LOG_INFO("All cures are researched - You won the game!");
        return false;
    }

    return true;
}

bool Game::gameOver()
{
    if (m_board.getNumOutbreaks() > c_maxOutbreaks)
    {
        LOG_INFO("Reached the maximum number of outbreaks!");
        return true;
    }
    else if (m_board.getNumPlayerCards() == 0) //BUGG
    {
        LOG_INFO("No more player cards left!");
        return true;
    }
    else if (false/*m_board.getNumPlacedDiseases() > c_maxPlacedDiseaseCubes*/)
    {
        // Todo: Add logic
        LOG_INFO("No more disease cubes left!");
        return true;
    }

    return false;
}

}
