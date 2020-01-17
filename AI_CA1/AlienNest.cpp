#include "AlienNest.h"

AlienNest::AlienNest(player& t_player, sf::Vector2f t_position) :
	m_player(t_player),
	m_isAlive{true},
	m_isMissileAlive{false},
	m_missileVelocity{ -MAX_VELOCITY, 0.0f },
	m_orientation{ 0.0f },
	m_targetPosition{ 0.0f, 0.0f }

{
	m_health = 4; // four hits from player 
	m_missileSpeed = t_player.getMaxSpeed();
	m_missileTTL = 5; // seconds to live if not hit player
	position = t_position;

	if (!m_texture.loadFromFile("./ASSETS/IMAGES/nest.png"))
	{
		std::string s("Error loading image");
		throw std::exception(s.c_str());
	} // loading image check

	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(position);
	m_sprite.setOrigin(sf::Vector2f(m_texture.getSize().x / 2, m_texture.getSize().y / 2));

}

AlienNest::~AlienNest()
{
}

void AlienNest::update(float t)
{
	if (m_isAlive) {
		// Calculating distance 
		if (sqrt(pow(position.x - m_player.playerPosition().x, 2) + pow(position.y - m_player.playerPosition().y, 2)) < 150) {
			if (!m_isMissileAlive) {
				std::cout << "fire" << std::endl;
				fire();
			}
		}
		if (m_health <= 0) {
			m_isAlive = false;
			m_texture.~Texture();
		}
	}
	if (m_isMissileAlive) {
		std::cout << m_missileTTL << std::endl;
		m_missileTTL -= t;
		if (m_missileTTL <= 0) {
			m_isMissileAlive = false;
			m_missileTTL = 5;
		}
		dynamicPursue(t / 1000);
		m_missileSprite.setPosition(m_missilePosition);
		m_missilePosition += m_missileVelocity;
		m_missileSprite.setRotation(m_orientation);
	}
}

void AlienNest::render(sf::RenderWindow& window)
{
	if (m_isAlive) {
		window.draw(m_sprite);
	}
	if (m_isMissileAlive) {
		window.draw(m_missileSprite);
	}
}

// when player is in range - fire  interceptor guided missiles
void AlienNest::fire()
{
	m_missilePosition = position;
	if (!m_missileTexture.loadFromFile("./ASSETS/IMAGES/missile.png"))
	{
		std::string s("Error loading image");
		throw std::exception(s.c_str());
	} // loading image check
	m_missileSprite.setTexture(m_missileTexture);
	m_missileSprite.setOrigin(sf::Vector2f(m_missileTexture.getSize().x / 2, m_missileTexture.getSize().y / 2));

	m_isMissileAlive = true;
}

void AlienNest::dynamicPursue(float t_deltaTime) {
	float timePrediction = 0.0f;
	sf::Vector2f direction = m_player.playerPosition() - m_missilePosition;	//direction = target.position - my.position
	float distance = Kinematic::vectorLength(direction);	//	distance = direction.length
	float speed = Kinematic::vectorLength(m_missileVelocity);		//	speed = my.velocity.length
	if (speed <= distance / MAX_TIME_PRED) { //	if speed <= distance / maxTimePrediction:
		timePrediction = MAX_TIME_PRED;	//timePrediction = maxTimeprediction
	}
	else {
		timePrediction = distance / speed;	//timePrediction = distance / speed
	}
	sf::Vector2f newTarget = m_player.playerPosition() + m_player.playerVelocity() * timePrediction * t_deltaTime;	//	newtarget.position = target.position + target.velocity * timePrediction
	kinematicSeek(t_deltaTime, newTarget); //	seek(me, newTarget)
}

void AlienNest::kinematicSeek(float t_deltaTime, sf::Vector2f& t_newTarget) {
	m_targetPosition = t_newTarget;
	m_missileVelocity = m_targetPosition - m_missilePosition;
	m_missileVelocity = m_missileVelocity / Kinematic::vectorLength(m_missileVelocity) ;
	m_missileVelocity *= MAX_VELOCITY;
	m_orientation = Kinematic::getNewOrientation(m_missilePosition, m_player.playerPosition());
}

sf::FloatRect AlienNest::boundingBox()
{
	//bounding box for collision
	sf::FloatRect boundingBox(m_sprite.getGlobalBounds().left + 10,
		m_sprite.getGlobalBounds().top + 10,
		m_sprite.getGlobalBounds().width - 10,
		m_sprite.getGlobalBounds().height - 10);
	return boundingBox;
}

sf::FloatRect AlienNest::missileBoundingBox()
{
	//bounding box for collision
	sf::FloatRect boundingBox(m_missileSprite.getGlobalBounds().left + 10,
		m_missileSprite.getGlobalBounds().top + 10,
		m_missileSprite.getGlobalBounds().width - 10,
		m_missileSprite.getGlobalBounds().height - 10);
	return boundingBox;
}

void AlienNest::damage()
{
	m_health--;
}

void AlienNest::destroyMissile()
{
	m_isMissileAlive = false;
}
