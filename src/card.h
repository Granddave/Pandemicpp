#pragma once

#include <memory>

#include "city.h"
#include "utils.h"

namespace pandemic
{
const int c_numEventCards = 5;

enum class EventCardType
{
    ResilientPopulation,
    Airlift,
    Forecast,
    OneQuietNight,
    GovernmentGrant
};

inline std::string eventCardToString(const EventCardType card)
{
    switch (card)
    {
        case EventCardType::ResilientPopulation:
            return "Resilient Population";
        case EventCardType::Airlift:
            return "Airlift";
        case EventCardType::Forecast:
            return "Forecast";
        case EventCardType::OneQuietNight:
            return "One Quiet Night";
        case EventCardType::GovernmentGrant:
            return "Government Grant";
    }

    VERIFY_NOT_REACHED();
    return "Unknown role";
}

// -----------------------------------------------------------------------------
// Player cards

struct PlayerCard
{
    PlayerCard() = default;
    virtual ~PlayerCard() = default;
    virtual std::string name() const = 0;
};

struct PlayerCityCard final : public PlayerCard
{
    explicit PlayerCityCard(std::shared_ptr<City> c) : city(std::move(c))
    {
    }

    std::string name() const override;

    std::shared_ptr<City> city;
};

struct EpidemicCard final : public PlayerCard
{
    std::string name() const override;
};

struct EventCard final : public PlayerCard
{
    explicit EventCard(const EventCardType et) : eventType(et)
    {
    }

    std::string name() const override;

    EventCardType eventType;
};

// -----------------------------------------------------------------------------
// Infection cards

struct InfectionCard
{
    explicit InfectionCard(std::shared_ptr<City> c) : city(c)
    {
    }

    std::shared_ptr<City> city;
};

} // namespace pandemic
