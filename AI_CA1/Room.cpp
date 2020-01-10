#include "Room.h"

room::room(sf::Vector2f size, sf::Vector2f position):
	m_size(size),
	m_position(position)
{
	m_roomRectangle.setSize(m_size);
	m_roomRectangle.setFillColor(sf::Color::White);
	m_roomRectangle.setPosition(m_position);
	m_roomRectangle.setOrigin(sf::Vector2f(m_size.x / 2, m_size.y / 2));
}

room::~room() {

}

bool room::isPlayerInRoom(sf::Vector2f playerSize, sf::Vector2f playerPosition) {
	if (playerPosition.x - playerSize.x / 2 < m_position.x - m_size.x / 2 ||
		playerPosition.x + playerSize.x / 2 > m_position.x + m_size.x / 2 ||
		playerPosition.y - playerSize.y / 2 < m_position.y - m_size.y / 2 ||
		playerPosition.y + playerSize.y / 2 > m_position.y + m_size.y / 2) {
		return false;
	}

	return true;
}

void room::render(sf::RenderWindow& m_window) {
	m_window.draw(m_roomRectangle);
}