#include "card.h"

std::string pandemic::PlayerCityCard::name() const
{
    return city->name();
}

std::string pandemic::EpidemicCard::name() const
{
    return "--Epidemic--";
}

std::string pandemic::EventCard::name() const
{
    return "Event: " + eventCardToString(eventType);
}
