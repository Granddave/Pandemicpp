#include "card.h"

namespace pandemic
{

std::string PlayerCityCard::name() const
{
    return city->name();
}

std::string EpidemicCard::name() const
{
    return "--Epidemic--";
}

std::string EventCard::name() const
{
    return "Event: " + eventCardToString(eventType);
}

} // namespace pandemic
