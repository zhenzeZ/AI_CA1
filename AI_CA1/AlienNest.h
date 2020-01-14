#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include "Player.h"
#include "Kinematic.h"

using namespace std;

class AlienNest {
public:
	AlienNest(player& player);
	~AlienNest();
	void update(float t);
	void render(sf::RenderWindow& window);


private:
	const short MAX_TIME_PRED = 1000; // ms

	sf::Vector2f m_targetPosition;
	sf::Vector2f m_missilePosition;
	sf::Vector2f m_missileVelocity;

	bool m_isMissileAlive;
	int m_health;
	int m_missileTTL;
	float m_missileSpeed;
	float rotation;
	float radian;
	float PI;

	float distanceToPlayer;

	sf::Vector2f velocity;
	sf::Vector2f position;

	sf::Texture m_texture;
	sf::Sprite m_sprite;

	sf::Vector2f m_target;

	float radius;

	float viewAngle;
	float viewRange;

	void fire(sf::Vector2f player, float t);
	void dynamicPursue(float t_deltaTime, sf::Vector2f& t_playerPos, sf::Vector2f& t_playerVelo);
	void kinematicSeek(float t_deltaTime, sf::Vector2f& t_playerPos);
};
