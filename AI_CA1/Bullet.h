#ifndef BULLET
#define BULLET
#pragma once

#include <SFML/Graphics.hpp>


class bullet
{
public:
	bullet(sf::Vector2f position, float radius);
	~bullet();

	void update(double t);
	void render(sf::RenderWindow& window);

	bool checkAlive() { return m_alive; }

	sf::FloatRect boundingBox();

	void setState(sf::Vector2f position, float radius);
private:

	void setUpTexture();

	sf::Texture m_bulletTexture;
	sf::Sprite m_bulletSprite; // palyer texture
	sf::Vector2f m_bulletPosition;
	sf::Vector2f m_velocity;

	float m_time;
	float speed;

	bool m_alive;

	float m_bulletRadius;
};

#endif // !BULLET
