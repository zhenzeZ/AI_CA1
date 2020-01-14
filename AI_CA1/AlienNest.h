#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include "Player.h"

using namespace std;

class AlienNest {
public:
	AlienNest(player& player);
	~AlienNest();
	void update(float t);
	void render(sf::RenderWindow& window);


private:
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
};