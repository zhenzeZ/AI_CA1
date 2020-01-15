#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

enum class AIStates
{
	Wander,
	Seek,
	Flee
};


using namespace std;

class sweeper {
public:
	sweeper(sf::Vector2f start, AIStates state);
	~sweeper();
	void update(sf::Vector2f worker, float t);
	void render(sf::RenderWindow& window);

private:
	AIStates m_aiStates;

	float speed;
	float rotation;
	float radian;
	float PI;

	float distanceToPlayer;

	sf::Vector2f velocity;
	sf::Vector2f position;

	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::ConvexShape m_triangle;

	sf::Vector2f m_viewEdge;

	sf::Vector2f m_target;

	sf::Font m_font;
	sf::Text m_behaviour;

	float radius;

	float viewAngle;
	float viewRange;

	void seek(sf::Vector2f player, float t);
	void wander(sf::Vector2f player, float t);
	void flee(sf::Vector2f player, float t);

	void vision(sf::Vector2f player);
};