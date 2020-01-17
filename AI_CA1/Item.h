#pragma once
#ifndef AMMO
#define AMMO
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class item
{
public:
	item(sf::Vector2f position, sf::Vector2i range);
	~item();

	void render(sf::RenderWindow& window);
	sf::FloatRect boundingBox();
	bool checkAlive() { return m_alive; }
	bool collisionCheck(sf::FloatRect player);
private:
	void setPosition();
	void setUpTexture();

	sf::Texture m_itemTexture;
	sf::Sprite m_itemSprite; // palyer texture
	sf::Vector2f m_itemPosition;

	bool m_alive;

};

#endif // !HEATLTH
