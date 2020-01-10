#pragma once
#include <SFML/Graphics.hpp>

class  room
{
public:
	room(sf::Vector2f size, sf::Vector2f position);
	~room();

	bool isPlayerInRoom(sf::Vector2f playerSize, sf::Vector2f playerPosition);
	void render(sf::RenderWindow& m_window);

private:
	sf::Vector2f m_size;
	sf::Vector2f m_position;

	sf::RectangleShape m_roomRectangle;

};