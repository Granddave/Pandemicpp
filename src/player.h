#pragma once

#include <memory>
#include <deque>

#include "card.h"
#include "city.h"

namespace pandemic {

const int c_minPlayers = 2;
const int c_maxPlayers = 4;
const int c_handLimit = 7;
const int c_numRoles = 7;

enum class ActionType
{
    Drive,
    DirectFly,
    CharterFlight,
    ShuttleFlight,
    BuildResearchStation,
    TreatDisease,
    ShareKnowledge,
    DiscoverCure
};

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

inline std::string actionToString(const ActionType type)
{
    switch (type)
    {
        case ActionType::Drive:                return "Drive";
        case ActionType::DirectFly:            return "DirectFly";
        case ActionType::CharterFlight:        return "CharterFlight";
        case ActionType::ShuttleFlight:        return "ShuttleFlight";
        case ActionType::BuildResearchStation: return "BuildResearchStation";
        case ActionType::TreatDisease:         return "TreatDisease";
        case ActionType::ShareKnowledge:       return "ShareKnowledge";
        case ActionType::DiscoverCure:         return "DiscoverCure";
    }
    return "Unknown action";
}

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
    }
    return "Unknown role";
}

class Player
{
public:
    void setCurrentCity(std::shared_ptr<City>& city);
    std::shared_ptr<City> currentCity() const { return m_currentCity; }
    void setRole(const Role role) { m_role = role; }
    Role role() const { return m_role; }
    void addCard(std::shared_ptr<PlayerCard>& card);
    std::deque<std::shared_ptr<PlayerCard>>& cards() { return m_cards; }

private:
    Role m_role;
    std::shared_ptr<City> m_currentCity;
    std::deque<std::shared_ptr<PlayerCard>> m_cards;
    // TODO: Make use of `int actionsLeft;`
};

struct ShareKnowledgeData
{
    std::shared_ptr<PlayerCityCard> cityCard;
    std::shared_ptr<Player> otherPlayer;
};

struct Action
{
    Action(ActionType action, std::shared_ptr<City> city)
        : action(action), city(city)
    {}
    Action(ActionType action, std::shared_ptr<PlayerCityCard> card, std::shared_ptr<Player> player = nullptr)
        : action(action), card(card), player(player)
    {}
    Action(ActionType action, DiseaseType type)
        : action(action), diseaseType(type)
    {}
    Action(ActionType action) : action(action) {}

    ActionType action = ActionType::Drive;
    std::shared_ptr<City> city;
    std::shared_ptr<PlayerCityCard> card;
    std::shared_ptr<Player> player;
    DiseaseType diseaseType = DiseaseType::Yellow;
};

}
