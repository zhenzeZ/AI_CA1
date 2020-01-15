#pragma once
#include <SFML/Graphics.hpp>

class  room
{
public:
	room(sf::Vector2f size, sf::Vector2f position);
	~room();

	bool isPlayerInRoom(sf::Vector2f playerSize, sf::Vector2f playerPosition);

	void render(sf::RenderWindow& m_window);

	sf::Vector2i getSize() { return sf::Vector2i(m_size); }
	sf::Vector2f getPosition() { return sf::Vector2f(m_position.x - m_size.x / 2 , m_position.y - m_size.y / 2); }
	sf::Vector2f getOrigin() { return m_position; }

private:
	sf::Vector2f m_size;
	sf::Vector2f m_position;

	sf::RectangleShape m_roomRectangle;

};