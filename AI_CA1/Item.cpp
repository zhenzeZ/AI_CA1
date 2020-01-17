#include "Item.h"
#include <math.h>

item::item(sf::Vector2f position, sf::Vector2i range)
{
	m_alive = false;
	setPosition(position, range);
	setUpTexture();
}

item::~item()
{

}


void item::setUpTexture()
{

	if (!m_itemTexture.loadFromFile("ASSETS/IMAGES/Item.png"))
	{
		std::string s("error loading ammo iamge from file");
		throw std::exception(s.c_str());
	}

	m_itemSprite.setTexture(m_itemTexture);
	m_itemSprite.setPosition(m_itemPosition);
	m_itemSprite.setOrigin(sf::Vector2f(m_itemSprite.getGlobalBounds().width / 2, m_itemSprite.getGlobalBounds().height / 2));
}

void item::render(sf::RenderWindow& window)
{
	window.draw(m_itemSprite);
}

sf::FloatRect item::boundingBox()
{
	//bounding box for collision
	sf::FloatRect boundingBox(m_itemSprite.getGlobalBounds().left + 2,
		m_itemSprite.getGlobalBounds().top + 2,
		m_itemSprite.getGlobalBounds().width - 5,
		m_itemSprite.getGlobalBounds().height - 5);
	return boundingBox;
}

bool item::collisionCheck(sf::FloatRect player)
{
	if (boundingBox().intersects(player) )
	{
		m_alive = false;
		return true;
	}

	return false;
}

void item::setPosition(sf::Vector2f position, sf::Vector2i range)
{
	m_itemPosition = sf::Vector2f(rand() % range.x + position.x, rand() % range.y + position.y);
	m_itemSprite.setPosition(m_itemPosition);
	m_alive = true;
}