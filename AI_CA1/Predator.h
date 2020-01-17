#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include "Player.h"
#include "Kinematic.h"

using namespace std;

class Predator {
public:
	Predator(player& player, sf::Vector2f t_position);
	~Predator();
	void update(float t);
	void render(sf::RenderWindow& window);

	sf::FloatRect boundingBox();

	/*sf::FloatRect boundingBox();
	void inTheRoom() { m_inRoom = true; }*/

private:
	/*void bounceOff();
	bool m_inRoom;
	float timer;*/

	player& m_player;
	const short MAX_TIME_PRED = 1000; // ms

	sf::Vector2f m_targetPosition;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_orientation;

	int m_health;
	float rotation;
	float radian;
	float PI;

	float distanceToPlayer;
	float distanceToPredator;

	sf::Texture m_texture;
	sf::Sprite m_sprite;

	float radius;

	float viewAngle;
	float viewRange;

	void attack(sf::Vector2f player, float t);
	void kinematicSeek(float t_deltaTime, sf::Vector2f t_playerPos);
};
