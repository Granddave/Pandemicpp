#pragma once

#include <memory>

#include "card.h"
#include "city.h"

namespace Pandemic {

const int c_handLimit = 7;

enum class Role
{
    ContingencyPlanner,
    Dispatcher,
    Medic,
    OperationsExpert,
    QuarantineSpecialist,
    Researcher,
    Scientist
};

class Player
{
public:
    Player();

    void setCurrentCity(std::shared_ptr<City> city);
    Role getRole() const { return m_role; }
    void addCard(std::shared_ptr<PlayerCard> card);
private:
    Role m_role;
    std::shared_ptr<City> m_currentCity;
    std::vector<std::shared_ptr<PlayerCard>> m_cards;
    int actionsLeft;
};

}
