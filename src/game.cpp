#include "game.h"

#include <algorithm>
#include <iostream>
#include <memory>

#include "logging.h"
#include "player.h"
#include "utils.h"

namespace pandemic
{
constexpr int c_numActionsPerTurn = 4;
constexpr int c_numPlayerCardsToDraw = 2;

Game::Game(Config config) : m_config(std::move(config))
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

    // FIXME: This seed does not work for modern random generators.
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
    m_round = 0;
}

void Game::run()
{
    while (shouldContinueGame())
    {
        LOG_INFO("--- Player {}'s turn - ", m_currentPlayerIx + 1);
        if (m_currentPlayerIx == 0)
        {
            m_round++;
            LOG_INFO("Round nr {}", m_round);
        }

        doTurn();
        m_currentPlayerIx = (m_currentPlayerIx + 1) % m_config.numPlayers;
    }
}

void Game::doTurn()
{
    auto currentPlayer = m_players.at(static_cast<size_t>(m_currentPlayerIx));

    for (int actionInTurnIx = 0; actionInTurnIx < c_numActionsPerTurn; ++actionInTurnIx)
    {
        printStatus();
        auto actions = possibleActions(currentPlayer);

        LOG_INFO("Currently in {}", currentPlayer->currentCity()->name());
        for (size_t actionIx = 0; actionIx < actions.size(); ++actionIx)
        {
            LOG_INFO("  {}: {}", actionIx + 1, actions.at(actionIx)->description());
        }

        std::string input;
        LOG_INFO("Action {}/{} - Choose an action...", actionInTurnIx + 1, c_numActionsPerTurn);
        std::cin >> input;
        auto option = static_cast<size_t>(std::stoi(input));
        VERIFY_LOG(option > 0, "Invalid input: {}", option);
        doAction(actions.at(option - 1));
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

        if (std::dynamic_pointer_cast<EpidemicCard>(drawnCard))
        {
            m_board.increaseInfectionRate();
            m_board.epidemicInfection();
            m_board.intensify();
        }
        else
        {
            currentPlayer->cards().push_back(drawnCard);
        }

        while (currentPlayer->cards().size() > c_handLimit)
        {
            // TODO: Let player choose card to discard
            // Random card will do for now...
            const int index = std::rand() % static_cast<int>(currentPlayer->cards().size());
            auto droppedCard =
                *currentPlayer->cards().erase(currentPlayer->cards().begin() + index);
            LOG_INFO("Dropping {} from hand", droppedCard->name());
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
    const auto currentPlayer = m_players.at(static_cast<size_t>(m_currentPlayerIx));
    LOG_INFO("Players:");
    for (const auto& player : m_players)
    {
        LOG_INFO("{}{} is in {}. Cards:",
                 currentPlayer == player ? "*" : "",
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
    VERIFY(numPlayers >= c_minPlayers);
    VERIFY(numPlayers <= c_maxPlayers);

    // Find start city
    const auto startCity = m_board.startCity();
    VERIFY(startCity);

    for (int playerIx = 0; playerIx < numPlayers; playerIx++)
    {
        auto player = std::make_shared<Player>();
        player->setCurrentCity(startCity);

        // Randomize the players role
        while (true)
        {
            Role role = static_cast<Role>(std::rand() % c_numRoles);
            auto roleCollision = std::find_if(
                m_players.begin(), m_players.end(), [&](const std::shared_ptr<Player>& p) {
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
        case Difficulty::Introductory:
            return 4;
        case Difficulty::Standard:
            return 5;
        case Difficulty::Heroic:
            return 6;
    }

    VERIFY_NOT_REACHED();
    return 4;
}

std::vector<std::shared_ptr<Action>> Game::possibleActions(
    const std::shared_ptr<Player>& currentPlayer) const
{
    std::vector<std::shared_ptr<Action>> actions;

    for (auto& city : currentPlayer->currentCity()->neighbours())
    {
        actions.push_back(std::make_shared<ActionDrive>(city));
    }

    std::vector<DiseaseType> cardTypes;
    for (const auto& playerCard : currentPlayer->cards())
    {
        if (auto cityCard = std::dynamic_pointer_cast<PlayerCityCard>(playerCard))
        {
            if (cityCard->city == currentPlayer->currentCity())
            {
                actions.push_back(std::make_shared<ActionCharterFly>(cityCard->city));

                if (!currentPlayer->currentCity()->hasResearchStation())
                {
                    actions.push_back(std::make_shared<ActionBuildResearchStation>());
                }

                // Share Knowledge
                for (const auto& otherPlayer : m_players)
                {
                    if (otherPlayer == currentPlayer)
                    {
                        continue;
                    }

                    if (otherPlayer->currentCity() == currentPlayer->currentCity())
                    {
                        actions.push_back(std::make_shared<ActionShareKnowledge>(
                            cityCard, currentPlayer, otherPlayer));
                    }
                }
            }
            else
            {
                actions.push_back(std::make_shared<ActionDirectFly>(cityCard->city));
            }

            cardTypes.push_back(cityCard->city->diseaseType());
        }
    }

    // Build research station
    if (currentPlayer->role() == Role::OperationsExpert &&
        !currentPlayer->currentCity()->hasResearchStation())
    {
        actions.push_back(std::make_shared<ActionBuildResearchStation>());
    }

    // Share Knowledge
    for (const auto& otherPlayer : m_players)
    {
        if (otherPlayer == currentPlayer)
        {
            continue;
        }

        for (const auto& playerCard : otherPlayer->cards())
        {
            const auto cityCard = std::dynamic_pointer_cast<PlayerCityCard>(playerCard);
            if (cityCard && cityCard->city == currentPlayer->currentCity())
            {
                actions.push_back(
                    std::make_shared<ActionShareKnowledge>(cityCard, otherPlayer, currentPlayer));
            }
        }
    }

    // Treat disease
    if (currentPlayer->currentCity()->numDiseaseCubes() > 0)
    {
        for (const DiseaseType diseaseType : currentPlayer->currentCity()->diseaseCubeTypes())
        {
            actions.push_back(std::make_shared<ActionTreatDisease>(diseaseType));
        }
    }

    if (currentPlayer->currentCity()->hasResearchStation())
    {
        // Shuttle flight
        const auto researchCities = m_board.researchStationCities();
        if (researchCities.size() > 1)
        {
            for (const auto& city : researchCities)
            {
                if (currentPlayer->currentCity() != city)
                {
                    actions.push_back(std::make_shared<ActionShuttleFly>(city));
                }
            }
        }

        // Discover cure
        for (int i = 0; i < c_numDiseases; ++i)
        {
            const auto diseaseType = static_cast<DiseaseType>(i);
            const auto cardCount = std::count(cardTypes.begin(), cardTypes.end(), diseaseType);
            const auto requiredCardCount =
                currentPlayer->role() == Role::Scientist ? c_numCardsToCure - 1 : c_numCardsToCure;
            if (cardCount >= requiredCardCount)
            {
                actions.push_back(std::make_shared<ActionDiscoverCure>(diseaseType));
            }
        }
    }

    // TODO: Add role specific actions
    // TODO: Add event actions

    return actions;
}

void Game::doAction(std::shared_ptr<Action> baseAction)
{
    auto currentPlayer = m_players.at(static_cast<size_t>(m_currentPlayerIx));

    if (auto action = std::dynamic_pointer_cast<ActionDrive>(baseAction))
    {
        auto city = action->city();
        LOG_INFO("Driving to {}", city->name());
        currentPlayer->setCurrentCity(city);
        return;
    }
    if (auto action = std::dynamic_pointer_cast<ActionDirectFly>(baseAction))
    {
        auto city = action->city();
        LOG_INFO("Flying directly to {}", city->name());
        currentPlayer->setCurrentCity(city);
        currentPlayer->removeCard(city);
        return;
    }
    if (auto action = std::dynamic_pointer_cast<ActionCharterFly>(baseAction))
    {
        auto city = action->city();
        LOG_INFO("Flying charter to {}", city->name());
        currentPlayer->setCurrentCity(city);
        currentPlayer->removeCard(city);
        return;
    }
    if (auto action = std::dynamic_pointer_cast<ActionShuttleFly>(baseAction))
    {
        auto city = action->city();
        LOG_INFO("Shuttle flying to {}", city->name());
        currentPlayer->setCurrentCity(city);
        return;
    }
    if (auto action = std::dynamic_pointer_cast<ActionBuildResearchStation>(baseAction))
    {
        auto city = currentPlayer->currentCity();
        VERIFY_LOG(!city->hasResearchStation(),
                   "Cant build research station since it already exists");
        LOG_INFO("Building research station in {}", city->name());
        city->setResearchStation(true);
        if (currentPlayer->role() != Role::OperationsExpert)
        {
            currentPlayer->removeCard(city);
        }
        // TODO: Only allow 6 research stations in total
        return;
    }
    if (auto action = std::dynamic_pointer_cast<ActionTreatDisease>(baseAction))
    {
        auto city = currentPlayer->currentCity();
        LOG_INFO("Treating {} disease in {}", diseaseToString(action->diseaseType()), city->name());

        city->treatDisease(action->diseaseType());
        // TODO: Medic ability
        // TODO: Cured diseases
        return;
    }
    if (auto action = std::dynamic_pointer_cast<ActionShareKnowledge>(baseAction))
    {
        LOG_INFO("Sharing knowledge for {} from {} to {}",
                 action->cityCard()->city->name(),
                 action->giver()->role(),
                 action->receiver()->role());

        auto card = std::dynamic_pointer_cast<PlayerCard>(action->cityCard());
        action->giver()->removeCard(card);
        action->receiver()->addCard(card);
        return;
    }
    if (auto action = std::dynamic_pointer_cast<ActionDiscoverCure>(baseAction))
    {
        m_board.discoverCure(action->diseaseType());
        // TODO: Discard cards from player
        return;
    }

    TODO();
}

bool Game::shouldContinueGame()
{
    if (isGameOver())
    {
        m_gameOver = true;
        LOG_INFO("Game Over!");
        return false;
    }
    if (m_board.numDiscoveredCures() == c_numCures)
    {
        m_gameWon = true;
        LOG_INFO("All cures are researched - You won the game!");
        return false;
    }

    return true;
}

bool Game::isGameOver()
{
    if (m_board.numOutbreaks() > c_maxOutbreaks)
    {
        LOG_INFO("Reached the maximum number of outbreaks!");
        return true;
    }
    if (m_board.numPlayerCards() == 0)
    {
        LOG_INFO("No more player cards left!");
        return true;
    }
    if (m_board.diseaseCubeCountMaxed())
    {
        LOG_INFO("No more disease cubes left!");
        return true;
    }

    return false;
}

} // namespace pandemic
