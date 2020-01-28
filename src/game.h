#pragma once

#include "board.h"
#include "card.h"
#include "player.h"

namespace Pandemic {


struct Config
{
    int numPlayers = 2;
    Difficulty difficulty = Difficulty::Easy;
    std::string citiesFile = "cities_data.txt";
    unsigned int seed = 1;
};

class Game
{
public:
    Game(const Config& config);

    void init();
    void reset();

    void printStatus();

private:
    void initPlayers(const int numPlayers);
    int numEpidemicCards(Difficulty difficulty) const;

    bool m_gameOver = false;
    Board m_board;
    Config m_config;
    std::vector<std::shared_ptr<Player>> m_players;
    int m_currentPlayer = 0;
};

}
