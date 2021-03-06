#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include "Player.h"
#include "Kinematic.h"
#include <cmath>
#include "Predator.h"

using namespace std;

class AlienNest {
public:
	AlienNest(Player& player, sf::Vector2f t_position);
	~AlienNest();
	void update(float t);
	void render(sf::RenderWindow& window);
	sf::FloatRect boundingBox();
	sf::FloatRect missileBoundingBox();
	void damage();
	sf::Vector2f getPosition() { return position; }
	void destroyMissile();
	vector<Predator*> getPredators() { return m_predators; }

	bool erasePredators(sf::FloatRect bullet);
private:
	float m_secondsPassed;
	vector<Predator*> m_predators;
	int m_noOfPredators;
	const int MAX_PREDATORS = 2;
	bool m_isAlive;
	Player& m_player;
	const short MAX_TIME_PRED = 1000; // ms

	sf::Vector2f m_targetPosition;
	sf::Vector2f m_missilePosition;
	sf::Vector2f m_missileVelocity;
	float m_orientation;

	bool m_isMissileAlive;
	int m_health;
	float m_missileTTL;
	float m_missileSpeed;
	float radian;
	float PI;

	float distanceToPlayer;

	sf::Vector2f velocity;
	sf::Vector2f position;

	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Texture m_missileTexture;
	sf::Sprite m_missileSprite;

	sf::Vector2f m_target;

	float radius;

	float viewAngle;
	float viewRange;

	void fire();
	void dynamicPursue(float t_deltaTime);
	void kinematicSeek(float t_deltaTime, sf::Vector2f& t_newTarget);
};
