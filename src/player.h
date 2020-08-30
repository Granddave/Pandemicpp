#pragma once

#include <deque>
#include <memory>

#include "card.h"
#include "city.h"

namespace pandemic
{
const int c_minPlayers = 2;
const int c_maxPlayers = 4;
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
        case Role::ContingencyPlanner:
            return "Contingency planner";
        case Role::Dispatcher:
            return "Dispatcher";
        case Role::Medic:
            return "Medic";
        case Role::OperationsExpert:
            return "Operations expert";
        case Role::QuarantineSpecialist:
            return "Quarantine specialist";
        case Role::Researcher:
            return "Researcher";
        case Role::Scientist:
            return "Scientist";
    }
    return "Unknown role";
}

class Player
{
public:
    void setCurrentCity(const std::shared_ptr<City>& city)
    {
        m_currentCity = city;
    }
    std::shared_ptr<City> currentCity() const
    {
        return m_currentCity;
    }
    void setRole(const Role role)
    {
        m_role = role;
    }
    Role role() const
    {
        return m_role;
    }
    void addCard(std::shared_ptr<PlayerCard>& card)
    {
        m_cards.push_back(card);
    }
    std::deque<std::shared_ptr<PlayerCard>>& cards()
    {
        return m_cards;
    }

private:
    Role m_role;
    std::shared_ptr<City> m_currentCity;
    std::deque<std::shared_ptr<PlayerCard>> m_cards;
    // TODO: Make use of `int actionsLeft;`
};

// --- Super classes ---
class Action
{
public:
    virtual ~Action() = default;
    virtual std::string description() = 0;
};

class ActionCity : public Action
{
public:
    ActionCity(std::shared_ptr<City> city);
    virtual ~ActionCity() override = default;

    virtual std::string description() override = 0;
    std::shared_ptr<City> city();

protected:
    std::shared_ptr<City> m_city;
};

// --- Sub classes ---
class ActionDrive : public ActionCity
{
public:
    ActionDrive(const std::shared_ptr<City>& city) : ActionCity(city)
    {
    }

    std::string description() override;
};

class ActionDirectFly : public ActionCity
{
public:
    ActionDirectFly(const std::shared_ptr<City>& city) : ActionCity(city)
    {
    }

    std::string description() override;
};

class ActionCharterFly : public ActionCity
{
public:
    ActionCharterFly(const std::shared_ptr<City>& city) : ActionCity(city)
    {
    }

    std::string description() override;
    // TODO: Add destination as well?
};

class ActionShuttleFly : public ActionCity
{
public:
    ActionShuttleFly(const std::shared_ptr<City>& city) : ActionCity(city)
    {
    }

    std::string description() override;
};

class ActionBuildResearchStation : public Action
{
public:
    ActionBuildResearchStation()
    {
    }

    std::string description() override;
};

class ActionTreatDisease : public Action
{
public:
    ActionTreatDisease(DiseaseType diseaseType) : m_diseaseType(diseaseType)
    {
    }

    std::string description() override;

private:
    DiseaseType m_diseaseType;
};

class ActionShareKnowledge : public Action
{
public:
    ActionShareKnowledge(std::shared_ptr<PlayerCityCard> cityCard,
                         std::shared_ptr<Player> otherPlayer);

    std::string description() override;
    std::shared_ptr<PlayerCityCard> cityCard();
    std::shared_ptr<Player> otherPlayer();

private:
    std::shared_ptr<PlayerCityCard> m_cityCard;
    std::shared_ptr<Player> m_otherPlayer;
};

class ActionDiscoverCure : public Action
{
public:
    ActionDiscoverCure()
    {
    }

    std::string description() override;
};

} // namespace pandemic
