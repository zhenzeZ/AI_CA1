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

void AlienNest::dynamicPursue(float t_deltaTime, sf::Vector2f& t_playerPos, sf::Vector2f& t_playerVelo) {
	float timePrediction = 0.0f;
	sf::Vector2f direction = t_playerPos - m_missilePosition;	//direction = target.position - my.position
	float distance = Kinematic::vectorLength(direction);	//	distance = direction.length
	float speed = Kinematic::vectorLength(m_missileVelocity);		//	speed = my.velocity.length
	if (speed <= distance / MAX_TIME_PRED) { //	if speed <= distance / maxTimePrediction:
		timePrediction = MAX_TIME_PRED;	//timePrediction = maxTimeprediction
	}
	else {
		timePrediction = distance / speed;	//timePrediction = distance / speed
	}
	sf::Vector2f newTarget = t_playerPos + t_playerVelo * timePrediction * t_deltaTime;	//	newtarget.position = target.position + target.velocity * timePrediction
	kinematicSeek(t_deltaTime, newTarget); //	seek(me, newTarget)
}

void AlienNest::kinematicSeek(float t_deltaTime, sf::Vector2f& t_playerPos) {
	m_targetPosition = t_playerPos;
	m_missileVelocity = m_targetPosition - m_missilePosition;
	m_missileVelocity = m_missileVelocity / Kinematic::vectorLength(m_missileVelocity) * t_deltaTime;
	m_missileVelocity *= MAX_VELOCITY;
}
