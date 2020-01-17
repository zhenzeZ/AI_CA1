#pragma once
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// @author Dion and Zhen
/// @date Jan 2020
/// Space station rescue game
/// </summary>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "AlienNest.h"
#include "Player.h"
#include "Room.h"
#include "Worker.h"
#include "Sweeper.h"
#include "Bullet.h"
#include "Item.h"

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:
	bool m_isGameOver;
	// the game view (full window)
	sf::View gameView; 
	
	// mini-map (upper-right corner)
	sf::View minimapView; 
	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();

	void setupMap();
	void setupWorkers();
	void setUpSweeper();
	void spawnItem();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_font; // font used by message
	sf::Text m_gameWon;

	AlienNest* m_alienNest;
	AlienNest* m_alienNest2;
	Player* m_player;
	room* m_room;
	worker* m_worker;
	sweeper* m_sweeper;
	item* m_item;

	vector<worker*> m_workers; 
	vector<bullet*> m_bullets;
	vector<room*> m_rooms;
	vector<sweeper*> m_sweepers;
	vector<item*> m_items;

	vector<sf::Vector2f> m_roomPosition;
	vector<sf::Vector2i> m_roomSize;

	float m_itemSpawnRate; // time to spawn item on the map

	bool m_exitGame; // control exiting game
	
	int m_numOfRoom; // number of rooms in the map
	int m_maxWorker; // max number of worker
	int m_maxSweeper; // max number of sweeper
};

#endif // !GAME_HPP

