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
	sweeper(sf::Vector2f start, vector<sf::Vector2f> roomPosition, vector<sf::Vector2i> roomSize, int current, sf::Font font);
	~sweeper();
	void update(sf::Vector2f player, float t);
	void vision(sf::Vector2f worker, int style);
	void render(sf::RenderWindow& window);

	void fleeState() { m_aiStates = AIStates::Flee; }
	void wanderState();

	void catchWorker() { m_worker++; }
	int getWorker() { return m_worker; }

	sf::FloatRect boundingBox();
	sf::Vector2f sweeperPosition() { return position; } // return current positon
	sf::Vector2f sweeperSize() { return size; }
	void inTheRoom() { m_inRoom = true; }

private:
	void bounceOff();
	bool m_inRoom;
	sf::Vector2f size;

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

	sf::Font m_font;
	sf::Text m_workerText;

	vector<sf::Vector2f> m_roomPosition;
	vector<sf::Vector2i> m_roomSize;

	int m_currentRoom;
	int m_nextRoom;
	int m_previousRoom;

	int m_searchCounter;

	void seek(sf::Vector2f worker, float t);
	void wander(sf::Vector2f target, float t);
	void flee(sf::Vector2f player, float t);

};