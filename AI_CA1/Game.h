#pragma once
/// <summary>
/// author Pete Lowe May 2019
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "AlienNest.h"
#include "Player.h"
#include "Room.h"
#include "Worker.h"
#include "Bullet.h"

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

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();

	void setupMap();
	void setupWorkers();

	sf::RenderWindow m_window; // main SFML window

	AlienNest* m_alienNest;
	player* m_player;
	room* m_room;
	worker* m_worker;

	vector<worker*> m_workers; 
	vector<bullet*> m_bullets;
	vector<room*> m_rooms;

	bool m_exitGame; // control exiting game

};

#endif // !GAME_HPP

