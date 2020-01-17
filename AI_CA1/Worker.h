#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

using namespace std;

class worker {
public:
	worker(sf::Vector2f start, sf::Vector2f position, sf::Vector2i range);
	~worker();
	void update(float t);
	void render(sf::RenderWindow& window);

	bool catchCheck(sf::Vector2f player);

	sf::FloatRect boundingBox();
	bool collisionCheck(sf::FloatRect object);

private:
	float speed;
	float rotation;
	float radian;
	float PI;

	float distanceToPlayer;

	sf::Vector2f velocity;
	sf::Vector2f position;

	sf::Texture m_texture;
	sf::Sprite m_sprite;

	sf::Vector2f m_target;

	sf::Vector2f m_randomOrigin;
	sf::Vector2i m_randomRange;

	float radius;

	float viewAngle;
	float viewRange;

	void wander(sf::Vector2f player, float t);
};