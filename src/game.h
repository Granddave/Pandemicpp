#pragma once

#include "board.h"
#include "card.h"
#include "player.h"

namespace Pandemic {

const int c_numActionsPerTurn = 4;
const int c_numPlayerCardsToDraw = 2;

struct Config
{
    int numPlayers = 2;
    Difficulty difficulty = Difficulty::Introductory;
    std::string citiesFile = "cities_data.txt";
    unsigned int seed = 0;
};

class Game
{
public:
    Game(const Config& config);

    void init();
    void reset();
    void run();
    void doTurn();

    void printStatus();

private:
    void initPlayers(const int numPlayers);
    int numEpidemicCards(Difficulty difficulty) const;
    std::vector<Action>possibleActions(const std::shared_ptr<Player>& player) const;
    bool continueGame();
    bool gameOver();

    bool m_gameOver = false;
    bool m_gameWon = false;
    Config m_config;
    Board m_board;
    std::vector<std::shared_ptr<Player>> m_players;
    int m_currentPlayerIx = 0;
    int m_round = 0;
};

}
