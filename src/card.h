#pragma once

#include <memory>

#include "city.h"

namespace Pandemic {

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
        case EventCardType::ResilientPopulation: return "Resilient Population";
        case EventCardType::Airlift:             return "Airlift";
        case EventCardType::Forecast:            return "Forecast";
        case EventCardType::OneQuietNight:       return "One Quiet Night";
        case EventCardType::GovernmentGrant:     return "Government Grant";
    }
    return "Unknown role";
}

// -----------------------------------------------------------------------------
// Player cards

struct PlayerCard
{
    PlayerCard() {}
    virtual ~PlayerCard() = default;
    virtual std::string getName() const = 0;
};

struct PlayerCityCard : PlayerCard
{
    PlayerCityCard(std::shared_ptr<City> city)
        : city(city)
    {
    }

    std::string getName() const override { return city->getName(); }

    std::shared_ptr<City> city;
    //int population;
};

struct EpidemicCard : PlayerCard
{
    std::string getName() const override { return "--Epidemic--"; }
};

struct EventCard : PlayerCard
{
    EventCard(const EventCardType eventType)
        : eventType(eventType)
    {
    }

    std::string getName() const override
    {
        return "Event: " + eventCardToString(eventType);
    }

    EventCardType eventType;
};

// -----------------------------------------------------------------------------
// Infection cards

struct InfectionCard
{
    InfectionCard(std::shared_ptr<City> city)
        : city(city)
    {
    }

    std::shared_ptr<City> city;
};

}
