#include "player.h"

namespace pandemic
{
ActionCity::ActionCity(std::shared_ptr<City> city) : m_city(std::move(city))
{
}

std::string ActionDrive::description()
{
    return "Drive to " + m_city->name();
}

std::string ActionDirectFly::description()
{
    return "Direct fly to " + m_city->name();
}

std::string ActionCharterFly::description()
{
    return "Charter fly from " + m_city->name();
}

std::string ActionShuttleFly::description()
{
    return "Shuttle fly to " + m_city->name();
}

std::string ActionBuildResearchStation::description()
{
    return "Build research station";
}

std::string ActionTreatDisease::description()
{
    return "Treat " + diseaseToString(m_diseaseType) + " disease";
}

std::string ActionShareKnowledge::description()
{
    return "Share knowledge of " + m_cityCard->name() + " from " + roleToString(m_giver->role()) +
           " to " + roleToString(m_receiver->role());
}

std::string ActionDiscoverCure::description()
{
    return "Discover cure";
}

} // namespace pandemic
