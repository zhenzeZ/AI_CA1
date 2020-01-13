#include "Bullet.h"
#include <math.h>

bullet::bullet(sf::Vector2f position, float radian)
{
	speed = 50.0f;
	m_time = 2.0f;
	m_alive = true;
	setUpTexture();

	setState(position, radian);
}

bullet::~bullet()
{

}


void bullet::setUpTexture()
{

	if (!m_bulletTexture.loadFromFile("ASSETS/IMAGES/BULLET.png"))
	{
		std::string s("error loading bullet iamge from file");
		throw std::exception(s.c_str());
	}

	m_bulletSprite.setTexture(m_bulletTexture);
	m_bulletSprite.setPosition(m_bulletPosition);
	m_bulletSprite.setOrigin(sf::Vector2f(m_bulletSprite.getGlobalBounds().width / 2, m_bulletSprite.getGlobalBounds().height / 2));
}

void bullet::update(double t)
{
	m_bulletPosition.x += m_velocity.x;
	m_bulletPosition.y += m_velocity.y;

	m_bulletSprite.setPosition(m_bulletPosition);

	m_time -= t;
	if (m_time <= 0) // turn the bullet off after 2 second
	{
		m_time = 2.0f;
		m_alive = false;
	}
}

void bullet::setState(sf::Vector2f position, float radius)
{

		m_bulletPosition = position;
		m_bulletRadius = radius;
		m_velocity = sf::Vector2f(speed * cos(m_bulletRadius), speed * sin(m_bulletRadius));	
}

void bullet::render(sf::RenderWindow& window)
{
	window.draw(m_bulletSprite);
}

sf::FloatRect bullet::boundingBox()
{
	//bounding box for collision
	sf::FloatRect boundingBox(m_bulletSprite.getGlobalBounds().left + 10,
		m_bulletSprite.getGlobalBounds().top + 10,
		m_bulletSprite.getGlobalBounds().width - 10,
		m_bulletSprite.getGlobalBounds().height - 10);
	return boundingBox;
}