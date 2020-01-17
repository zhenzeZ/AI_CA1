#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

class player {
public:
	player(sf::Vector2f start, sf::Font font);
	~player();
	void update(float t);
	void render(sf::RenderWindow& window);

	void saveWorker(int i) { m_workerSaved += i; }

	void playerInTheRoom() { m_playerInRoom = true; }

	sf::Vector2f playerPosition() { return position; } // return current positon
	sf::Vector2f playerSize() { return size; }
	sf::Vector2f pursuePosition() { return pursue; } // return pursue position
	sf::Vector2f playerVelocity() { return velocity; } // return pursue position

	float playerRadian() { return radian; }

	void ammoChanged(int ammo) { m_ammo += ammo; }
	int getAmmo() { return m_ammo; }
	int getHealth() { return m_health; }

	sf::FloatRect boundingBox();

	void buttonCheck();
	void powerUps(int itemStyle);

	float m_fireRate;

	float getMaxSpeed() { return maxSpeed; }
	void damage();
private:
	void bounceOff();

	void setUpText();
	void setUpShape();

	float acceleration;
	float rotation;
	float radian;
	float maxSpeed;
	float PI;

	float speed;
	float pursueTime;

	int m_workerSaved;
	int m_ammo;
	float m_health;
	float m_energy;

	sf::Vector2f velocity;
	sf::Vector2f moveDistance;
	sf::Vector2f position;
	sf::Vector2f size;

	sf::Vector2f pursue;


	sf::RectangleShape m_HealthBar[2]; // 1 for inside bar, 0 for out line edge

	sf::Font m_font;

	sf::Text m_ammoText;
	sf::Text m_workersText;

	sf::Text m_gameoverText;

	bool accelerating;

	bool m_playerInRoom;

	//sf::Vector2f motion;
	//sf::Vector2f m_acceleration;

	sf::Texture m_texture;
	sf::Sprite m_sprite;

	sf::Texture m_barrierTexture;
	sf::Sprite m_barrier;
	bool m_barrierOn;
	float m_barrierTimer;

	void movementCalculate(float t);
};