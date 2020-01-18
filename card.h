#pragma once

#include <memory>

#include "city.h"


namespace Pandemic {

struct PlayerCard
{
    PlayerCard() {}
    virtual ~PlayerCard() = default;
    virtual std::string getName() = 0;
};

struct PlayerCityCard : PlayerCard
{
    PlayerCityCard(std::shared_ptr<City> city)
        : city(city)
    {
    }

    std::string getName() { return city->getName(); }

    std::shared_ptr<City> city;
    //int population;
};

struct EpidemicCard : PlayerCard
{
    std::string getName() { return "Epidemic"; }
    // Todo
};

struct EventCard : PlayerCard
{
    std::string getName() { return "Event card"; }
    // Todo
};

struct InfectionCard
{
    InfectionCard(std::shared_ptr<City> city)
        : city(city)
    {
    }
    std::shared_ptr<City> city;
};

}
