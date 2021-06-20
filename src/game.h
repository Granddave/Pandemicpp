#pragma once

#include "board.h"
#include "card.h"
#include "player.h"

namespace pandemic
{
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
    explicit Game(Config);

    void init();
    void reset();
    void run();
    void doTurn();

    void printStatus();

private:
    void initPlayers(const int numPlayers);
    int numEpidemicCards(Difficulty) const;
    std::vector<std::shared_ptr<Action>> possibleActions(const std::shared_ptr<Player>&) const;
    void doAction(std::shared_ptr<Action>);
    bool shouldContinueGame();
    bool isGameOver();

    bool m_gameOver = false;
    bool m_gameWon = false;
    Config m_config;
    Board m_board;
    std::vector<std::shared_ptr<Player>> m_players;
    int m_currentPlayerIx = 0;
    int m_round = 0;
};

} // namespace pandemic
