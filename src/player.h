#pragma once

#include <deque>
#include <memory>

#include "card.h"
#include "city.h"
#include "utils.h"

namespace pandemic
{
constexpr int c_minPlayers = 2;
constexpr int c_maxPlayers = 4;
constexpr int c_handLimit = 7;
constexpr int c_numRoles = 7;

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

    VERIFY_NOT_REACHED();
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
    void removeCard(std::shared_ptr<PlayerCard>& card)
    {
        LOG_DEBUG("Removing playercard: {} from {}", card->name(), roleToString(m_role));
        const auto rmIt = std::find(m_cards.begin(), m_cards.end(), card);
        if (rmIt != m_cards.cend())
        {
            m_cards.erase(rmIt);
            return;
        }

        VERIFY_NOT_REACHED_LOG("Failed to find card in players stack");
    }
    void removeCard(std::shared_ptr<City>& city)
    {
        LOG_DEBUG("Removing citycard: {} from {}", city->name(), roleToString(m_role));
        const auto rmIt = std::find_if(
            m_cards.begin(), m_cards.end(), [&city](std::shared_ptr<PlayerCard>& card) {
                if (auto cityCard = std::dynamic_pointer_cast<PlayerCityCard>(card))
                {
                    return cityCard->city == city;
                }
                return false;
            });

        if (rmIt != m_cards.cend())
        {
            m_cards.erase(rmIt);
            return;
        }

        VERIFY_NOT_REACHED_LOG("Failed to find card in players stack");
    }

private:
    Role m_role{};
    std::shared_ptr<City> m_currentCity;
    std::deque<std::shared_ptr<PlayerCard>> m_cards;
    // TODO: Make use of `int actionsLeft;`
};

// --- Super classes ---
class Action
{
public:
    Action() = default;
    virtual ~Action() = default;
    virtual std::string description() = 0;
};

class ActionCity : public Action
{
public:
    explicit ActionCity(std::shared_ptr<City> city);
    virtual ~ActionCity() = default;

    std::shared_ptr<City> city() const
    {
        return m_city;
    };

protected:
    std::shared_ptr<City> m_city;
};

// --- Sub classes ---
class ActionDrive final : public ActionCity
{
public:
    explicit ActionDrive(const std::shared_ptr<City>& city) : ActionCity(city)
    {
    }

    std::string description() override;
};

class ActionDirectFly final : public ActionCity
{
public:
    explicit ActionDirectFly(const std::shared_ptr<City>& city) : ActionCity(city)
    {
    }

    std::string description() override;
};

class ActionCharterFly final : public ActionCity
{
public:
    explicit ActionCharterFly(const std::shared_ptr<City>& city) : ActionCity(city)
    {
    }

    std::string description() override;
    // TODO: Add destination as well?
};

class ActionShuttleFly final : public ActionCity
{
public:
    explicit ActionShuttleFly(const std::shared_ptr<City>& city) : ActionCity(city)
    {
        VERIFY(city->hasResearchStation());
    }

    std::string description() override;
};

class ActionBuildResearchStation final : public Action
{
public:
    ActionBuildResearchStation() = default;
    std::string description() override;
};

class ActionTreatDisease final : public Action
{
public:
    explicit ActionTreatDisease(DiseaseType diseaseType) : m_diseaseType(diseaseType)
    {
    }

    std::string description() override;
    DiseaseType diseaseType() const
    {
        return m_diseaseType;
    }

private:
    DiseaseType m_diseaseType;
};

class ActionShareKnowledge final : public Action
{
public:
    explicit ActionShareKnowledge(std::shared_ptr<PlayerCityCard> cityCard,
                                  std::shared_ptr<Player> giver,
                                  std::shared_ptr<Player> receiver)
        : m_cityCard(std::move(cityCard)),
          m_giver(std::move(giver)),
          m_receiver(std::move(receiver))
    {
    }

    std::string description() override;
    std::shared_ptr<PlayerCityCard> cityCard()
    {
        return m_cityCard;
    }
    std::shared_ptr<Player> giver()
    {
        return m_giver;
    }
    std::shared_ptr<Player> receiver()
    {
        return m_receiver;
    }

private:
    std::shared_ptr<PlayerCityCard> m_cityCard;
    std::shared_ptr<Player> m_giver;
    std::shared_ptr<Player> m_receiver;
};

class ActionDiscoverCure final : public Action
{
public:
    explicit ActionDiscoverCure(DiseaseType diseaseType) : m_diseaseType(diseaseType)
    {
    }

    std::string description() override;
    DiseaseType diseaseType() const
    {
        return m_diseaseType;
    }

private:
    DiseaseType m_diseaseType;
};

} // namespace pandemic
