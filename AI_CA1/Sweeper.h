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
	sweeper(sf::Vector2f start, vector<sf::Vector2f> roomPosition, int current);
	~sweeper();
	void update(sf::Vector2f player, sf::Vector2f worker , float t);
	void render(sf::RenderWindow& window);

	void inPlayerSight() { m_aiStates = AIStates::Flee; }

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

	float radius;

	float viewAngle;
	float viewRange;

	int m_worker;

	vector<sf::Vector2f> m_roomPosition;
	int m_currentRoom;
	int m_nextRoom;
	int previousRoom;

	void seek(sf::Vector2f worker, float t);
	void wander(sf::Vector2f target, float t);
	void flee(sf::Vector2f player, float t);

	void vision(sf::Vector2f worker);
};