#include "Predator.h"

Predator::Predator(player& player, sf::Vector2f t_position) :
	m_player{player}
{
	if (!m_texture.loadFromFile("./ASSETS/IMAGES/zenomorph.png"))
	{
		std::string s("Error loading image");
		throw std::exception(s.c_str());
	} // loading image check
	m_position = t_position;
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(sf::Vector2f(m_texture.getSize().x / 2, m_texture.getSize().y / 2));
}

Predator::~Predator()
{
}

void Predator::update(float t)
{
	kinematicSeek(t / 1000, m_player.playerPosition());
	m_sprite.setPosition(m_position);
	m_position += m_velocity;
	m_sprite.setRotation(m_orientation);
}

void Predator::render(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}

void Predator::attack(sf::Vector2f player, float t)
{
}

void Predator::kinematicSeek(float t_deltaTime, sf::Vector2f t_newTarget) 
{
	m_targetPosition = t_newTarget;
	m_velocity = m_targetPosition - m_position;
	m_velocity = m_velocity / Kinematic::vectorLength(m_velocity);
	m_velocity *= MAX_VELOCITY;
	m_orientation = Kinematic::getNewOrientation(m_position, m_player.playerPosition());
}
