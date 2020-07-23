#include "player.h"

namespace pandemic
{

void Player::setCurrentCity(std::shared_ptr<City>& city)
{
    m_currentCity = city;
}

void Player::addCard(std::shared_ptr<PlayerCard>& card)
{
    m_cards.push_back(card);
}

} // namespace pandemic
