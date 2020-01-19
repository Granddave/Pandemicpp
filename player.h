#pragma once

#include <memory>
#include <deque>

#include "card.h"
#include "city.h"

namespace Pandemic {

const int c_handLimit = 7;
const int c_numRoles = 7;

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

inline std::string roleToString(const Role role)
{
    switch (role)
    {
        case Role::ContingencyPlanner:   return "Contingency planner";
        case Role::Dispatcher:           return "Dispatcher";
        case Role::Medic:                return "Medic";
        case Role::OperationsExpert:     return "Operations expert";
        case Role::QuarantineSpecialist: return "Quarantine specialist";
        case Role::Researcher:           return "Researcher";
        case Role::Scientist:            return "Scientist";
        default: return "Unknown role";
    }
}

class Player
{
public:
    void setCurrentCity(std::shared_ptr<City> city);
    void setRole(const Role role) { m_role = role; }
    Role getRole() const { return m_role; }
    void addCard(std::shared_ptr<PlayerCard> card);
private:
    Role m_role;
    std::shared_ptr<City> m_currentCity;
    std::deque<std::shared_ptr<PlayerCard>> m_cards;
    int actionsLeft;
};

}
