#include "card.h"

std::string Pandemic::PlayerCityCard::name() const
{
    return city->name();
}

std::string Pandemic::EpidemicCard::name() const
{
    return "--Epidemic--";
}

std::string Pandemic::EventCard::name() const
{
    return "Event: " + eventCardToString(eventType);
}
