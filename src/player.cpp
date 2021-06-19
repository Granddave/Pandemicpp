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

ActionShareKnowledge::ActionShareKnowledge(std::shared_ptr<PlayerCityCard> cityCard,
                                           std::shared_ptr<Player> otherPlayer)
    : m_cityCard(std::move(cityCard)), m_otherPlayer(std::move(otherPlayer))
{
}

std::string ActionShareKnowledge::description()
{
    return "Share knowledge of " + m_cityCard->name() + " to " +
           roleToString(m_otherPlayer->role());
}

std::shared_ptr<PlayerCityCard> ActionShareKnowledge::cityCard()
{
    return m_cityCard;
}

std::shared_ptr<Player> ActionShareKnowledge::otherPlayer()
{
    return m_otherPlayer;
}

std::string ActionDiscoverCure::description()
{
    return "Discover cure";
}

} // namespace pandemic
