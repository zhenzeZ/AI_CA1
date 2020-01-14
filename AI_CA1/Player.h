#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>

class player {
public:
	player(sf::Vector2f start);
	~player();
	void update(float t);
	void render(sf::RenderWindow& window);

	void saveWorker() { m_workerSaved++; }

	sf::Vector2f playerPosition() { return position; } // return current positon
	sf::Vector2f playerSize() { return size; }
	sf::Vector2f pursuePosition() { return pursue; } // return pursue position

	float playerRadian() { return radian; }

	void buttonCheck();
	float getMaxSpeed() { return maxSpeed; }
private:
	float acceleration;
	float rotation;
	float radian;
	float maxSpeed;
	float PI;

	float speed;
	float pursueTime;

	float m_workerSaved;

	sf::Vector2f velocity;
	sf::Vector2f moveDistance;
	sf::Vector2f position;
	sf::Vector2f size;

	sf::Vector2f pursue;

	sf::View m_view;

	bool accelerating;

	//sf::Vector2f motion;
	//sf::Vector2f m_acceleration;

	sf::Texture m_texture;
	sf::Sprite m_sprite;

	void movementCalculate(float t);
};