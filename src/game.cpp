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
    const auto seed = [&]() {
        if (m_config.seed == 0)
        {
            return static_cast<unsigned int>(time(nullptr));
        }
        return m_config.seed;
    }();
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
    m_currentPlayerIx = 0;
}

void Game::run()
{
    while (continueGame())
    {
        LOG_INFO("--- Player {}'s turn - ", m_currentPlayerIx+1);
        if (m_currentPlayerIx == 0)
        {
            m_round++;
            LOG_INFO("Round nr {}", m_round);
        }

        doTurn();
        m_currentPlayerIx = (m_currentPlayerIx+1) % m_config.numPlayers;
    }
}

void Game::doTurn()
{
    auto currentPlayer = m_players.at(static_cast<size_t>(m_currentPlayerIx));
    for (int i = 0; i < c_numActionsPerTurn; ++i)
    {
        LOG_TRACE("Action {}", i+1);
        auto actions = possibleActions(currentPlayer);

        // Todo: Let player choose action
    }

    // Draw two player cards
    for (int i = 0; i < c_numPlayerCardsToDraw; ++i)
    {
        auto drawnCard = m_board.drawPlayerCard();
        if (!drawnCard)
        {
            // No cards left, game over
            return;
        }

        LOG_INFO("Picked up {}", drawnCard->name());

        auto& playersCards = currentPlayer->cards();
        if (std::dynamic_pointer_cast<EpidemicCard>(drawnCard))
        {
            m_board.increaseInfectionRate();
            m_board.epidemicInfection();
            m_board.intensify();
        }
        else
        {
            playersCards.push_back(drawnCard);
        }

        while (playersCards.size() > c_handLimit)
        {
            // Todo: Let player choose card to discard
            // Random card will do for now...
            int index = std::rand() % static_cast<int>(playersCards.size());
            auto droppedCard = playersCards.erase(playersCards.begin() + index);
            LOG_INFO("Dropping {}", (*droppedCard)->name());
        }
    }

    // Infect cities
    for (int i = 0; i < m_board.infectionRate(); ++i)
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
                 roleToString(player->role()),
                 player->currentCity()->name());
        for (const auto& card : player->cards())
        {
            LOG_INFO(" * {}", card->name());
        }
    }

    LOG_INFO("- Diseases:");
    for (const auto& city : m_board.cities())
    {
        const int numCubes = city->numDiseaseCubes();
        if (numCubes > 0)
        {
            LOG_INFO("{} - {}", city->name(), numCubes);
        }
    }
}

void Game::initPlayers(const int numPlayers)
{
    assert(numPlayers >= c_minPlayers);
    assert(numPlayers <= c_maxPlayers);

    // Find start city
    auto cities = m_board.cities();
    auto startCity = m_board.startCity();
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
                return p->role() == role;
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
        case Difficulty::Introductory: return 4;
        case Difficulty::Standard:     return 5;
        case Difficulty::Heroic:       return 6;
    }
    return 4;
}

std::vector<Action> Game::possibleActions(const std::shared_ptr<Player>& player) const
{
    std::vector<Action> actions;

    for (auto& city : player->currentCity()->neighbours())
    {
        actions.emplace_back(ActionType::Drive, city);
    }

    std::vector<DiseaseType> cardTypes;
    for (const auto& p : player->cards())
    {
        auto cityCard = std::dynamic_pointer_cast<PlayerCityCard>(p);
        if (cityCard != nullptr)
        {
            actions.emplace_back(ActionType::DirectFly, cityCard);

            if (cityCard->city == player->currentCity())
            {
                actions.emplace_back(ActionType::CharterFlight, cityCard);

                if (!player->currentCity()->hasResearchStation())
                {
                    actions.emplace_back(ActionType::BuildResearchStation);
                }

                // Todo: Share Knowledge - Give card to other player
            }

            cardTypes.push_back(cityCard->city->diseaseType());
        }
    }

    if (!player->currentCity()->hasResearchStation() &&
        player->role() == Role::OperationsExpert)
    {
        actions.emplace_back(ActionType::BuildResearchStation);
    }

    /* Todo: Share Knowledge -
     *
     * if (another player is here)
     *     for players card
     *         if (card == currentCity)
     *             Share knowledge
     */

    if (player->currentCity()->hasResearchStation())
    {
        const auto researchCities = m_board.researchStationCities();
        if (researchCities.size() > 1)
        {
            for (const auto& city : researchCities)
            {
                actions.emplace_back(ActionType::ShuttleFlight, city);
            }
        }

        for (int i = 0; i < c_numDiseases; ++i)
        {
            const auto type = static_cast<DiseaseType>(i);
            const auto cardCount = std::count(cardTypes.begin(), cardTypes.end(), type);
            const auto requiredCardCount = player->role() == Role::Scientist
                                     ? c_numCardsToCure-1
                                     : c_numCardsToCure;
            if (cardCount >= requiredCardCount)
            {
                actions.push_back(ActionType::TreatDisease);
            }
        }
    }

    for (const auto& a : actions)
    {
        LOG_TRACE(actionToString(a.action));
    }

    return actions;
}

bool Game::continueGame()
{
    if (gameOver())
    {
        m_gameOver = true;
        LOG_INFO("Game Over!");
        return false;
    }
    else if (m_board.numDiscoveredCures() == c_numCures)
    {
        m_gameWon = true;
        LOG_INFO("All cures are researched - You won the game!");
        return false;
    }

    return true;
}

bool Game::gameOver()
{
    if (m_board.numOutbreaks() > c_maxOutbreaks)
    {
        LOG_INFO("Reached the maximum number of outbreaks!");
        return true;
    }
    else if (m_board.numPlayerCards() == 0)
    {
        LOG_INFO("No more player cards left!");
        return true;
    }
    else if (m_board.diseaseCubeCountMaxed())
    {
        LOG_INFO("No more disease cubes left!");
        return true;
    }

    return false;
}

}
