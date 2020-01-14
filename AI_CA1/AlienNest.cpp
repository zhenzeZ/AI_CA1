#include "AlienNest.h"

AlienNest::AlienNest(player& t_player)
{
	m_health = 4; // four hits from player 
	m_missileSpeed = t_player.getMaxSpeed();
	m_missileTTL = 5; // seconds to live if not hit player
	m_isMissileAlive = false; // can only be one at any one time
}

AlienNest::~AlienNest()
{
}

void AlienNest::update(float t)
{
}

void AlienNest::render(sf::RenderWindow& window)
{
}

// when player is in range - fire  interceptor guided missiles
void AlienNest::fire(sf::Vector2f player, float t)
{
}
